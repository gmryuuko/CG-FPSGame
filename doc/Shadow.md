# 阴影

## Shadow mapping

增加bias可以解决由于深度贴图精度不足带来的Shadow Acne，但是又会带来悬浮（Peter Panning）问题。可以使用正面剔除解决，但只对封闭物体有效。

事实上使用了正面剔除之后一般并不需要增加Shadow bias.

## 软阴影