#ifndef FRC2014_CAMERA_HPP_INCLUDED
#define FRC2014_CAMERA_HPP_INCLUDED

namespace frc
{
    class camera
    {
    private:
        //the current image
        frc::image * currentImage;
        frc::image * writingBuffer;
    public:
        ///@brief the camera object allows the capturing of an image at a specific rate
        ///
        ///the camera uses frame timing to know when it should update next
        camera();
        ///@brief performs the necassary captures, and swaps buffers when needed
        void update(float & lastFrameTime);
        ///@brief holds the current image, can be compared to the last used image to see if an update has occured, multithreaded mehtod would be prefered
        frc::image * getImage();
    };
}

#endif // FRC2014_CAMERA_HPP_INCLUDED
