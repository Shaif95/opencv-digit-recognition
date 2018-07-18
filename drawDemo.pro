TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    mousedraw.cpp

INCLUDEPATH += D:/sdk/opencv/opencv/build/include \
INCLUDEPATH += D:/sdk/opencv/opencv/build/include/opencv
INCLUDEPATH += $$PWD



LIBS += -LD:/sdk/opencv/opencv/build/x64/vc12/lib \
    -lopencv_core249d \
    -lopencv_highgui249d \
    -lopencv_imgproc249d \
    -lopencv_features2d249d \
    -lopencv_calib3d249d \
    -lopencv_contrib249d \
    -lopencv_flann249d \
    -lopencv_gpu249d \
    -lopencv_legacy249d \
    -lopencv_ml249d \
    -lopencv_nonfree249d \
    -lopencv_objdetect249d \
    -lopencv_ocl249d \
    -lopencv_photo249d \
    -lopencv_stitching249d \
    -lopencv_superres249d \
    -lopencv_ts249d \
    -lopencv_video249d \
    -lopencv_videostab249d


#INCLUDEPATH += D:/sdk/opencv3/opencv/buildm/include \
#INCLUDEPATH += D:/sdk/opencv3/opencv/buildm/include/opencv

#LIBS += -LD:/sdk/opencv3/opencv/buildm/x64/vc12/lib \
#	-lopencv_bgsegm300d \
#	-lopencv_bioinspired300d \
#	-lopencv_calib3d300d \
#	-lopencv_ccalib300d \
#	-lopencv_core300d \
#	-lopencv_datasets300d \
#	-lopencv_face300d \
#	-lopencv_features2d300d \
#	-lopencv_flann300d \
#	-lopencv_hal300d \
#	-lopencv_highgui300d \
#	-lopencv_imgcodecs300d \
#	-lopencv_imgproc300d \
#	-lopencv_latentsvm300d \
#	-lopencv_line_descriptor300d \
#	-lopencv_ml300d \
#	-lopencv_objdetect300d \
#	-lopencv_optflow300d \
#	-lopencv_photo300d \
#	-lopencv_reg300d \
#	-lopencv_rgbd300d \
#	-lopencv_saliency300d \
#	-lopencv_shape300d \
#	-lopencv_stereo300d \
#	-lopencv_stitching300d \
#	-lopencv_superres300d \
#	-lopencv_surface_matching300d \
#	-lopencv_text300d \
#	-lopencv_tracking300d \
#	-lopencv_ts300d \
#	-lopencv_video300d \
#	-lopencv_videoio300d \
#	-lopencv_videostab300d \
#	-lopencv_xfeatures2d300d \
#	-lopencv_ximgproc300d \
#	-lopencv_xobjdetect300d \
#	-lopencv_xphoto300d

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    mousedraw.h

