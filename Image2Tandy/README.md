# Image2Tandy
This is a small tool to be used to convert image files (PNG, JPEG, BMP, etc..) to image files in the tandy format.
The tool will convert the input image into 16 colors.

| RGB | Color |
|------|------|
|0 0 0 |       black|
|0 0 170   |   blue|
|0 170 0  |    green|
|0 170 170  |  cyan|
|170 0 0   |   red|
|170 0 170 |   magenta|
|170 85 0   |  brown|
|170 170 170  |light gray|
|85 85 85  |   gray|
|85 85 255  |  light blue|
|85 255 85  |  light green|
|85 255 255  | light cyan|
|255 85 85   | light red|
|255 85 255  | light magenta|
|255 255 85 |  yellow|
|255 255 255 |  white|

There is also a GIMP Palette file here that contains the 16 tandy colors: [Tandy16colors.gpl](Tandy16colors.gpl)