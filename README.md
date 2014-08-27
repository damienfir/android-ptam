# Log file format

lines 1-3:
    corners of the object area
line 4:
    timestamp for beep emission
line 5-:
    timestamp followed by recorded position of camera

The positions are recorded as transformation matrix which defines the model-view transformation from the plane to the camera.
The transformation matrix contains translation and rotation information.
The 3x4 matrix is serialized in a row-major format:
    T11 T12 T13 T14 T21 T22 T23 T24 T31 T32 T33 T34

The timestamp is the milliseconds since epoch (01.01.1970) and serves as a relative measure.
