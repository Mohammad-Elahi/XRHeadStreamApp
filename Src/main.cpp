#include <cstdint>
#include <cstdio>
#include <array>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>

#include "XrApp.h"
#include "Input/TinyUI.h"

#include "motionPredictionLinearSeparate.hpp"

#include <iostream>

#include <jni.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>

// Passthrough-related function pointers
PFN_xrCreatePassthroughFB xrCreatePassthroughFB = nullptr;
PFN_xrDestroyPassthroughFB xrDestroyPassthroughFB = nullptr;
PFN_xrPassthroughStartFB xrPassthroughStartFB = nullptr;
PFN_xrPassthroughPauseFB xrPassthroughPauseFB = nullptr;
PFN_xrCreatePassthroughLayerFB xrCreatePassthroughLayerFB = nullptr;
PFN_xrDestroyPassthroughLayerFB xrDestroyPassthroughLayerFB = nullptr;
PFN_xrPassthroughLayerSetStyleFB xrPassthroughLayerSetStyleFB = nullptr;
PFN_xrPassthroughLayerPauseFB xrPassthroughLayerPauseFB = nullptr;
PFN_xrPassthroughLayerResumeFB xrPassthroughLayerResumeFB = nullptr;

class XrBodyFaceEyeSocialApp : public OVRFW::XrApp {
public:
    XrBodyFaceEyeSocialApp() : OVRFW::XrApp() {
        BackgroundColor = OVR::Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
    }

    // Returns a list of OpenXr extensions needed for this app
    virtual std::vector<const char*> GetExtensions() override {
        std::vector<const char*> extensions = XrApp::GetExtensions();
        extensions.push_back(XR_FB_PASSTHROUGH_EXTENSION_NAME);
        return extensions;
    }

    virtual bool AppInit(const xrJava* context) override {
        if (false == ui_.Init(context, GetFileSys())) {
            ALOG("TinyUI::Init FAILED.");
            return false;
        }

        std::unique_ptr<OVRFW::ovrFileSys> fileSys = std::unique_ptr<OVRFW::ovrFileSys>(OVRFW::ovrFileSys::Create(*context));
        std::vector<uint8_t> bufferOrient;
        std::vector<double> doubles;
        std::vector<uint8_t> bufferTrans;

        if (fileSys) {
            std::string environmentPath = "apk:///assets/weightsOrientation.txt";
            if (!fileSys->ReadFile(environmentPath.c_str(), bufferOrient)) {
                ALOGW("Failed to load uri '%s'", environmentPath.c_str());
            }
            std::string environmentPathTrans = "apk:///assets/weightsTranslation.txt";
            if (!fileSys->ReadFile(environmentPathTrans.c_str(), bufferTrans)) {
                ALOGW("Failed to load uri '%s'", environmentPathTrans.c_str());
            }
        }

        motionpred.provideWeightsFileRaw(bufferOrient, bufferTrans);

        struct sockaddr_in dest;
        pose_send_socket = socket(AF_INET, SOCK_DGRAM, 0);
        memset(&dest, 0, sizeof(dest));
        dest.sin_family = AF_INET;
        dest.sin_addr.s_addr = inet_addr("192.168.137.1");
        uint32_t sendPort = 23001;
        dest.sin_port = htons(sendPort);
        int ret = connect(pose_send_socket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));

        // Initialize Passthrough function pointers
        OXR(xrGetInstanceProcAddr(GetInstance(), "xrCreatePassthroughFB", (PFN_xrVoidFunction*)&xrCreatePassthroughFB));
        OXR(xrGetInstanceProcAddr(GetInstance(), "xrDestroyPassthroughFB", (PFN_xrVoidFunction*)&xrDestroyPassthroughFB));
        OXR(xrGetInstanceProcAddr(GetInstance(), "xrPassthroughStartFB", (PFN_xrVoidFunction*)&xrPassthroughStartFB));
        OXR(xrGetInstanceProcAddr(GetInstance(), "xrPassthroughPauseFB", (PFN_xrVoidFunction*)&xrPassthroughPauseFB));
        OXR(xrGetInstanceProcAddr(GetInstance(), "xrCreatePassthroughLayerFB", (PFN_xrVoidFunction*)&xrCreatePassthroughLayerFB));
        OXR(xrGetInstanceProcAddr(GetInstance(), "xrDestroyPassthroughLayerFB", (PFN_xrVoidFunction*)&xrDestroyPassthroughLayerFB));
        OXR(xrGetInstanceProcAddr(GetInstance(), "xrPassthroughLayerSetStyleFB", (PFN_xrVoidFunction*)&xrPassthroughLayerSetStyleFB));
        OXR(xrGetInstanceProcAddr(GetInstance(), "xrPassthroughLayerPauseFB", (PFN_xrVoidFunction*)&xrPassthroughLayerPauseFB));
        OXR(xrGetInstanceProcAddr(GetInstance(), "xrPassthroughLayerResumeFB", (PFN_xrVoidFunction*)&xrPassthroughLayerResumeFB));

        return (ret >= 0);
    }

    virtual void AppShutdown(const xrJava* context) override {
        // Save collected data to CSV file
        SaveHeadTrackingDataToCSV();

        ui_.Shutdown();
        OVRFW::XrApp::AppShutdown(context);
    }

    virtual bool SessionInit() override {
        if (!OVRFW::XrApp::SessionInit()) {
            return false;
        }

        // Create Passthrough
        XrPassthroughCreateInfoFB ptci = {XR_TYPE_PASSTHROUGH_CREATE_INFO_FB};
        OXR(xrCreatePassthroughFB(GetSession(), &ptci, &passthrough));

        // Create Passthrough Layer
        XrPassthroughLayerCreateInfoFB plci = {XR_TYPE_PASSTHROUGH_LAYER_CREATE_INFO_FB};
        plci.passthrough = passthrough;
        plci.purpose = XR_PASSTHROUGH_LAYER_PURPOSE_RECONSTRUCTION_FB;
        OXR(xrCreatePassthroughLayerFB(GetSession(), &plci, &passthroughLayer));

        // Start Passthrough
        OXR(xrPassthroughStartFB(passthrough));
        OXR(xrPassthroughLayerResumeFB(passthroughLayer));

        return true;
    }

    virtual void SessionEnd() override {
        // Stop and destroy Passthrough
        if (passthrough != XR_NULL_HANDLE) {
            OXR(xrPassthroughPauseFB(passthrough));
            OXR(xrDestroyPassthroughLayerFB(passthroughLayer));
            OXR(xrDestroyPassthroughFB(passthrough));
            passthrough = XR_NULL_HANDLE;
            passthroughLayer = XR_NULL_HANDLE;
        }

        ui_.Shutdown();
        OVRFW::XrApp::SessionEnd();
    }

    virtual void Update(const OVRFW::ovrApplFrameIn& in) override {
        // Collect head tracking data
        CollectHeadTrackingData(in);

        ui_.Update(in);
    }

    virtual void Render(const OVRFW::ovrApplFrameIn& in, OVRFW::ovrRendererOutput& out) override {
        ui_.Render(in, out);

        // Add Passthrough layer to the frame
        XrCompositionLayerPassthroughFB passthrough_layer = {XR_TYPE_COMPOSITION_LAYER_PASSTHROUGH_FB};
        passthrough_layer.layerHandle = passthroughLayer;
        passthrough_layer.flags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT;
        passthrough_layer.space = XR_NULL_HANDLE;

        // Add the Passthrough layer to the layers list
        layerList.push_back(reinterpret_cast<XrCompositionLayerBaseHeader*>(&passthrough_layer));
    }

    void fillCharWithDouble(char* inBuf, double inVal) {
        unsigned char const * p = reinterpret_cast<unsigned char const *>(&inVal);

        for (std::size_t i = 0; i != sizeof(double); ++i)
        {
            inBuf[i] = p[i];
        }
        return;
    }

    void fillCharWithFloat(char* inBuf, float inVal) {
        unsigned char const * p = reinterpret_cast<unsigned char const *>(&inVal);

        for (std::size_t i = 0; i != sizeof(float); ++i)
        {
            inBuf[i] = p[i];
        }
        return;
    }

    bool sendPoseData(const Pose& poseData) {
        constexpr size_t bufferSize = 8 + (3+4)*4;
        char bufferOut[bufferSize];
        fillCharWithDouble(bufferOut, poseData.timestamp);
        fillCharWithFloat((bufferOut+8), poseData.position.x);
        fillCharWithFloat((bufferOut+12), poseData.position.y);
        fillCharWithFloat((bufferOut+16), poseData.position.z);
        fillCharWithFloat((bufferOut+20), poseData.orientation.w);
        fillCharWithFloat((bufferOut+24), poseData.orientation.x);
        fillCharWithFloat((bufferOut+28), poseData.orientation.y);
        fillCharWithFloat((bufferOut+32), poseData.orientation.z);

        int ret = send(pose_send_socket, bufferOut, bufferSize, 0);

        return (ret > 0);
    }

private:
    OVRFW::TinyUI ui_;
    int pose_send_socket;
    motionPredctionLinearSeparate motionpred = motionPredctionLinearSeparate();
    XrPassthroughFB passthrough = XR_NULL_HANDLE;
    XrPassthroughLayerFB passthroughLayer = XR_NULL_HANDLE;
    std::vector<XrCompositionLayerBaseHeader*> layerList;

    struct HeadTrackingData {
        int64_t timestamp;
        float posX, posY, posZ;
        float rotQx, rotQy, rotQz, rotQw;
    };

    std::vector<HeadTrackingData> headTrackingData;

    void CollectHeadTrackingData(const OVRFW::ovrApplFrameIn& in) {
        XrSpaceLocation viewLocation{XR_TYPE_SPACE_LOCATION};
        OXR(xrLocateSpace(GetHeadSpace(), GetStageSpace(), ToXrTime(in.PredictedDisplayTime), &viewLocation));

        if ((viewLocation.locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT) != 0 &&
            (viewLocation.locationFlags & XR_SPACE_LOCATION_ORIENTATION_VALID_BIT) != 0) {

            HeadTrackingData data;
            data.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
            data.posX = viewLocation.pose.position.x;
            data.posY = viewLocation.pose.position.y;
            data.posZ = viewLocation.pose.position.z;
            data.rotQx = viewLocation.pose.orientation.x;
            data.rotQy = viewLocation.pose.orientation.y;
            data.rotQz = viewLocation.pose.orientation.z;
            data.rotQw = viewLocation.pose.orientation.w;

            quaternion q;
            q.timestamp = ((double) data.timestamp) * 1e-3;
            q.w = viewLocation.pose.orientation.w;
            q.x = viewLocation.pose.orientation.x;
            q.y = viewLocation.pose.orientation.y;
            q.z = viewLocation.pose.orientation.z;

            position p;
            p.timestamp = ((double) data.timestamp) * 1e-3;
            p.x = viewLocation.pose.position.x;
            p.y = viewLocation.pose.position.y;
            p.z = viewLocation.pose.position.z;

            Pose testPose;
            testPose.timestamp = p.timestamp;
            testPose.orientation = q;
            testPose.position = p;

            sendPoseData(testPose);
        }
    }

    void SaveHeadTrackingDataToCSV() {
        const std::string filePath = "/storage/emulated/0/Download/HeadsetTrackingData.csv";
        std::ofstream file(filePath);

        if (file.is_open()) {
            file << "Timestamp,PositionX,PositionY,PositionZ,RotationQx,RotationQy,RotationQz,RotationQw\n";

            for (const auto& data : headTrackingData) {
                file << data.timestamp << ","
                     << data.posX << "," << data.posY << "," << data.posZ << ","
                     << data.rotQx << "," << data.rotQy << "," << data.rotQz << "," << data.rotQw << "\n";
            }

            file.close();
            ALOG("Head tracking data saved to: %s", filePath.c_str());
        } else {
            ALOG("Failed to open file for saving head tracking data");
        }
    }
};

ENTRY_POINT(XrBodyFaceEyeSocialApp);