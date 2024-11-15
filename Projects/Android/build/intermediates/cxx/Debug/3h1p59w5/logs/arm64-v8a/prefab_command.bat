@echo off
"C:\\Program Files\\Android\\Android Studio\\jbr\\bin\\java" ^
  --class-path ^
  "C:\\Users\\Mohammad.Elahi\\.gradle\\caches\\modules-2\\files-2.1\\com.google.prefab\\cli\\2.0.0\\f2702b5ca13df54e3ca92f29d6b403fb6285d8df\\cli-2.0.0-all.jar" ^
  com.google.prefab.cli.AppKt ^
  --build-system ^
  cmake ^
  --platform ^
  android ^
  --abi ^
  arm64-v8a ^
  --os-version ^
  29 ^
  --stl ^
  c++_static ^
  --ndk-version ^
  26 ^
  --output ^
  "C:\\Users\\MOHAMM~1.ELA\\AppData\\Local\\Temp\\agp-prefab-staging10403789083674029731\\staged-cli-output" ^
  "C:\\Users\\Mohammad.Elahi\\.gradle\\caches\\transforms-3\\386b20fbfb01919d12d6530576df6af8\\transformed\\openxr_loader_for_android-1.1.41\\prefab"
