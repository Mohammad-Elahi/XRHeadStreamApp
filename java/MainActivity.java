// Copyright (c) Facebook Technologies, LLC and its affiliates. All Rights reserved.
package com.oculus.xrsamples.xrbodyfaceeyesocial;

import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends android.app.NativeActivity {
  private static final String PERMISSION_EYE_TRACKING = "com.oculus.permission.EYE_TRACKING";
  private static final String PERMISSION_FACE_TRACKING = "com.oculus.permission.FACE_TRACKING";
  private static final String PERMISSION_RECORD_AUDIO = "android.permission.RECORD_AUDIO";
  private static final String PERMISSION_INTERNET = "android.permission.INTERNET";
  private static final String PERMISSION_NETWORK_STATE = "android.permission.ACCESS_NETWORK_STATE";
  private static final int REQUEST_CODE_PERMISSION_EYE_AND_FACE_TRACKING = 1;
  private static final String PERMISSIONS[] = {"com.oculus.permission.USE_SCENE"};
  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    Log.d("main","test does this work?"+PackageManager.PERMISSION_GRANTED);
    Log.d("main","check internet?" + checkSelfPermission(PERMISSION_INTERNET));
    if (checkSelfPermission(PERMISSION_INTERNET) != PackageManager.PERMISSION_GRANTED) {
      Log.d("main","no internet?");
//      permissionsToRequest.add(PERMISSION_INTERNET);
    }

    // This sample makes use of eye and face tracking data at all times
    // so we are requesting it in onCreate. If an app, only requires eye
    // and face tracking data deep in their experience it may make sense
    // to query this later in the app lifecycle.
    requestFaceEyeTrackingPermissionsIfNeeded();
  }

  private void requestFaceEyeTrackingPermissionsIfNeeded() {
    List<String> permissionsToRequest = new ArrayList<>();
    if (checkSelfPermission(PERMISSION_EYE_TRACKING) != PackageManager.PERMISSION_GRANTED) {
      permissionsToRequest.add(PERMISSION_EYE_TRACKING);
    }
    if (checkSelfPermission(PERMISSION_FACE_TRACKING) != PackageManager.PERMISSION_GRANTED) {
      permissionsToRequest.add(PERMISSION_FACE_TRACKING);
    }
    if (checkSelfPermission(PERMISSION_RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
      permissionsToRequest.add(PERMISSION_RECORD_AUDIO);
    }

    Log.d("main","check internet?");
    if (checkSelfPermission(PERMISSION_INTERNET) != PackageManager.PERMISSION_GRANTED) {
      Log.d("main","no internet?");
      permissionsToRequest.add(PERMISSION_INTERNET);
    }
    if (checkSelfPermission(PERMISSION_NETWORK_STATE) != PackageManager.PERMISSION_GRANTED) {
      permissionsToRequest.add(PERMISSION_NETWORK_STATE);
    }

    if (!permissionsToRequest.isEmpty()) {
      String[] permissionsAsArray =
          permissionsToRequest.toArray(new String[permissionsToRequest.size()]);
      requestPermissions(permissionsAsArray, REQUEST_CODE_PERMISSION_EYE_AND_FACE_TRACKING);
    }
  }
}
