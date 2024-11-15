@echo off
"C:\\Program Files\\Android\\Android Studio\\jbr\\bin\\java" ^
  --class-path ^
  "C:\\Users\\Mohammad.Elahi\\.gradle\\caches\\modules-2\\files-2.1\\com.google.prefab\\cli\\2.1.0\\aa32fec809c44fa531f01dcfb739b5b3304d3050\\cli-2.1.0-all.jar" ^
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
  "C:\\Users\\MOHAMM~1.ELA\\AppData\\Local\\Temp\\agp-prefab-staging11358886059263626816\\staged-cli-output" ^
  "C:\\Users\\Mohammad.Elahi\\.gradle\\caches\\8.9\\transforms\\b795aee036288d3e07e23a10ad7f1b07\\transformed\\openxr_loader_for_android-1.1.42\\prefab"
