# _QLabellingArrangement_

_QLabellingArrangement_ is a simple GUI to label images. It was initially developed to label façade images for training algorithms. Since version 1.2.0 (see Versions notes below), _QLabellingArrangement_ is based on CGAL::Arrangement_2 for faces definitions and is thus not anymore restricted to label axis-aligned objects.

## How you can help
If you find a bug or wish to have a new feature, please fill an issue in the dedicated section ([issues][issues_page]). If you want to code yourself a new feature, ask for a pull-request and detail your changes / modifications / improvements.

## Versions notes
### Version 1.2.0 (04/07/2013)
* Complete code reorganisation
* Labelling is now based on CGAL::Arrangement_2 and is thus not limited to axis-aligned rectangles
* Several bug fixes
* Add FR_fr translation

### Version 1.1.0 (15/05/2013)
* Dynamic labels (can be choosed at runtime from a simple ASCII file; see files rc/labels_facade_simple.txt or rc/labels_facade_enhanced.txt for examples)
* Fixes issues #2, #3, #4

## Authors
_QLabelling_ has been developed within the IMAGINE research group. See [IMAGINE web page][id_imagine_domain] for more details.

[id_imagine_domain]: http://imagine.enpc.fr/ "IMAGINE web page"
[issues_page]: https://github.com/oliviertournaire/QLabelling/issues "issues"