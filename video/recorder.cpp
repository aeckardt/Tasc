#include "recorder.h"

#include "image/screenshot.h"

void ScreenRecorder::captureFrame()
{
    // The linesize needs to be aligned with 32 bytes
    // Unfortunately that corrupts the QImage,
    // you see it for instance when you save it as png
    // it becomes erroneous!
    captureRect(rect, video->nextFrame(), 32);
    video->encodeFrame();
}

void ScreenRecorder::exec(QRect rect, Video &video, int frame_rate)
{
#ifndef __APPLE
    // Width / height need to be aligned by a factor of 2 for video encoding
    // -> on MacOS this step can be skipped
    //    as to why see further down this function!
    rect.setSize(QSize(rect.width() & 0xfffe, rect.height() & 0xfffe));
#endif

    if (rect.width() == 0 || rect.height() == 0)
        // Cannot record empty frames
        return;

    this->rect = rect;
    this->video = &video;
    this->frame_rate = frame_rate;

    if (frame_rate > 0 && frame_rate <= 30)
        interval = 1000 / frame_rate;
    else if (frame_rate <= 0)
        interval = 1000;
    else // if (frame_rate > 30)
        interval = 33;

    int width = rect.width();
    int height = rect.height();

#ifdef __APPLE__
    // On MacOS, the size of the screenshot is 2x the area of the screen
    width  *= 2;
    height *= 2;
#endif

    video.create(width, height, frame_rate);

    //

    //    hotkey.setShortcut(hotkeySequence);

//    startWorkers();

    // Capture one frame directly at start
    captureFrame();

//    hotkey.setRegistered(true);

//    timer.setInterval(interval);
//    timer.start();

//    timer.stop();
//    hotkey.setRegistered(false);

//    stopWorkers();

    video.flush();
}
