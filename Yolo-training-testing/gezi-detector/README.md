# Unofficial notes

`cv2.dnn.readNetFromDarkNet("<.cfg file>", "<.weights darknet weights file>")`
> Note: use `yolov3-spp.cfg` from `../yolov3/cfg`   
> weights must be converted from `.pt` to `.weights`

```bash
# convert cfg/pytorch model to darknet weights
$ python3  -c "from models import *; convert('cfg/yolov3-spp.cfg', 'weights/yolov3-spp.pt')"

```
