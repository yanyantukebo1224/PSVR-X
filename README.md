# PSVR-X

Experimental PCVR driver and tracking stack for the original PlayStation VR (PSVR).

**Current status: early development scaffold.** This repository does not yet provide a working SteamVR HMD driver or 6DoF tracking.

## Target hardware

- Original PSVR headset
- PS4 Camera mounted on the top of the headset
- Windows 10/11
- [PSCam4Windows](https://github.com/BumbleTree/PSCam4Windows) exposing the PS4 Camera as a Windows virtual camera
- SteamVR

## Planned pipeline

```text
PS4 Camera (stereo, headset-mounted)
        |
        v
PSCam4Windows virtual camera
        |
        v
PSVR-X camera capture + stereo calibration
        |
        v
Stereo visual odometry / visual-inertial tracking
        |
        v
SteamVR OpenVR driver (HMD pose + display)
```

The first milestone is deliberately small: establish a reliable, testable stereo-frame representation and verify the camera's side-by-side layout. Tracking is not considered implemented until it has been validated against real headset motion.

## Camera assumptions

PSCam4Windows documents the PS4 Camera as a stereo device. Depending on its selected output mode, the two sensors may be exposed in a side-by-side frame or as separate virtual cameras. PSVR-X must not silently assume one layout: the capture configuration must be explicit and checked against actual frames.

The camera is mounted to the HMD and therefore observes the environment from the headset's moving viewpoint. Room-fixed visual features are needed for visual odometry. Stereo calibration is required before metric depth or reliable stereo pose estimation can be claimed.

## Build and test

The initial core library has no third-party dependencies.

```powershell
cmake -S . -B build -DPSVRX_BUILD_TESTS=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Visual Studio 2022 (C++ workload) or another C++17-capable compiler and CMake 3.20+ are recommended.

## Development stages

- [x] Repository scaffold and stereo-frame validation core
- [ ] Inspect real PSCam4Windows output and add a camera capture backend
- [ ] Stereo calibration tool and saved calibration format
- [ ] Visual odometry / SLAM with relocalization and tracking-loss handling
- [ ] Investigate PSVR IMU access and visual-inertial fusion
- [ ] OpenVR SteamVR driver, HMD display integration, and pose reporting
- [ ] End-to-end tests with the physical PSVR and PS4 Camera

## Important limitations

- A camera-only prototype is not equivalent to a complete VR headset driver.
- Stereo vision can help recover metric scale, but it does not by itself guarantee robust tracking.
- PSVR display, USB/IMU communication, lens/display timing, distortion, and SteamVR integration are separate engineering tasks.
- Do not use this prototype for safety-critical movement or in situations where loss of tracking could cause injury.

## License

The project license has not yet been selected. PSCam4Windows is a separate GPL-licensed project; this repository currently treats it as an external camera provider and does not copy its implementation.
