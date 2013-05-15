# _QLabelling_

_QLabelling_ is a simple GUI to label rectified images. It was initially developed to label façade images for training algorithms.

## What does _QLabelling_ do
* Draw horizontal and vertical lines with a single click (intersections define rectangles to label)
* Labels rectangles
* Choose a color for each label
* Save a labels image with associated information (labels' colors, input and label images) in a raw ASCII file <image_label_filename>.labels
* Dynamic labels: you must defines labels names and colors in a separate text file. See files rc/labels_facade_simple.txt or rc/labels_facade_enhanced.txt for examples. **Note that you always must to define a label named _unknow_ (case sensitive)!**

## What _QLabelling_ does not do
* Delete lines or rectangles
* Merge rectangles

## How you can help
If you find a bug or wish to have a new feature, please fill an issue in the dedicated section ([issues][issues_page]). If you want to code yourself a new feature, ask for a pull-request and detail your changes / modifications / improvements.

## Versions notes
### Version 1.1.0 (15/05/2013)
* Dynamic labels (can be choosed at runtime from a simple ASCII file; see files rc/labels_facade_simple.txt or rc/labels_facade_enhanced.txt for examples)
* Fixes issues #2, #3, #4

## Authors
_QLabelling_ has been developed within the IMAGINE research group. See [IMAGINE web page][id_imagine_domain] for more details.

[id_imagine_domain]: http://imagine.enpc.fr/ "IMAGINE web page"
[issues_page]: https://github.com/oliviertournaire/QLabelling/issues "issues"