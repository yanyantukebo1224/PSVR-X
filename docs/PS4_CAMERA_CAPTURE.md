# PS4 Camera capture validation

## Confirmed user setup

The current PSCam4Win configuration screenshot shows:

- Device: PS4 Camera
- Stream: 1280 x 400 at approximately 120.5 fps
- View: Side-by-side
- Split into two cameras: disabled
- Horizontal and vertical flips: disabled

PSVR-X should initially treat this as one packed frame containing two 640 x 400 eye images. This is a configuration assumption to verify against actual frames, not a hard-coded truth about every PSCam4Win configuration.

## Capture requirements

The Windows capture backend should:

1. Enumerate Media Foundation video capture devices.
2. Display each friendly name and symbolic link so the user can identify the PSCam4Win virtual camera.
3. Enumerate native media types, including subtype, frame size, and frame-rate numerator/denominator.
4. Select the configured device by symbolic link, not a fragile device index.
5. Request the native 1280 x 400 mode where available; do not silently scale or reinterpret another mode.
6. Record sample timestamps and report gaps / duplicate timestamps.
7. Validate that the delivered frame is 1280 x 400 before splitting into two 640 x 400 views.
8. Keep camera capture independent from the tracking thread; a slow SLAM frame must not block the camera reader.

## Important timing note

The control panel's approximately 120.5 fps display is not proof that every application receives a stable 120.5 fps stream. The capture backend must calculate the observed interval from Media Foundation sample timestamps and report the measured rate and discontinuities.

## First acceptance test

With PSCam4Win streaming and the headset stationary, the diagnostic tool should show:

- Correct virtual-camera device name
- 1280 x 400 delivered frame size
- Two valid 640 x 400 eye regions
- Observed frame interval / rate
- Count of timestamp gaps and invalid frames

Do not start SLAM until this test passes.
