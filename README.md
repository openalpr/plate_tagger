# OpenALPR Plate Tagger

This utility is used to annotate images with the location and numbers of the license plates.  These annotated images can be used to train the OpenALPR algorithm to recognize license plates accurately.

The utility is a QT graphical user interface written in C++:
![Plate Image](http://www.openalpr.com/images/demoscreenshots/tagger_screenshot.jpg "Input image")

## Installation

Download an installer for Windows, Ubuntu Linux, or Mac OS from the Releases tab.  Alternatively, you can compile the tool yourself using `qmake` or `cmake`. Eg. on a Fedora-based system (assuming that the required tools are already installed):

```bash
$ git clone https://github.com/openalpr/plate_tagger.git
$ cd plate_tagger
$ mkdir build && cd build
$ cmake ..
$ make
$ ./openalpr_tagger
```

