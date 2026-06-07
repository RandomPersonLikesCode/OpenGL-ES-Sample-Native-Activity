# Guides
## About

This is the guide if you are curious of how to
make a game or just tinkering with OpenGL ES (though you can
also do it with Vulkan if you prefer) on Android just with Termux and
your own phone only with C (C++ is also possible here, though the build time would
be slower).

## Build Device Prerequisites

- Architecture is AArch64
- RAM is atleast 4GB (recommended)
- Free storage atleast 4GB

## Test Device Prerequisites

- Android 7.0 (minimum for OpenGL ES 3.2)
- GPU atleast supports for OpenGL ES 3.2 (recommended)

## Termux Tools Prerequisites

- AAPT2 (from Termux)
- Clang Toolchain (from NDK)
- Zip
- 7z
- Zipalign
- Keytool
- Apksigner
- Wget

```sh
apt update && apt upgrade
apt install aapt aapt2 zip p7zip apksigner openjdk-21 wget
```

## Termux Directories

```sh
mkdir -p $HOME/.local/share/android/{cmdline-tools,ndk}
```

## Termux Environment Variables

```sh
export JAVA_HOME="$PREFIX/lib/jvm/java-21-openjdk"
export ANDROID_HOME="$HOME/.local/share/android"
export ANDROID_NDK_HOME="$ANDROID_HOME/ndk"

export PATH="$PATH:$ANDROID_HOME/cmdline-tools/latest/bin"
```

# Downloading The SDK/NDK
## SDK

```sh
cd $HOME/.local/share/android/cmdline-tools
wget <link-to-cmdline-tools>
unzip <cmdline-tools-zipped-file>.zip
mv <cmdline-tools-unzipped-folder> latest

sdkmanager --licenses
sdkmanager "platforms;android-34"

# <> is variable, make sure to replace it
```

## NDK

```sh
cd $HOME/.|ocal/share/android/ndk

wget https://github.com/lzhiyong/termux-ndk/releases/download/android-ndk/android-ndk-<version>-aarch64.7z
7z x <7zipped-file>.7z
mv <unzipped-folder> <version>

# <> is variable, make sure to replace it
```

# Build this project

```sh
cd <project-root-dir>

keytool -genpair -keystore <file>.p12 -alias <alias> -keyalg RSA -keysize 2048 -storetype PKCS12 -validity 365
KS=<file>.p12 KS_ALIAS=<alias> KS_PASS=<password> COPY= ./scripts/build

# <> is variable, make sure to replace it
# make sure to to replace variables in ./scripts/build to
# respected NDK version, target compilation, SDK version, etc
```
