#ifndef CAPTURE_H
#define CAPTURE_H

#include <TooN/TooN.h>
#include <TooN/se3.h>

class Capture
{
public:
    Capture ();
    virtual ~Capture ();

    bool store_corner(TooN::SE3<>);
    void store_position(TooN::SE3<>);
    void reset_corners();
    void reset_positions();
    std::vector<TooN::Vector<3> > get_rectangle();
    std::vector<TooN::Vector<3> > get_positions();
    std::vector<TooN::SE3<> > get_corners_matrices();

private:
    std::vector<TooN::Vector<3> > _corners;
    std::vector<TooN::SE3<> > _corners_matrices;
    std::vector<TooN::Vector<3> > _positions;
    std::vector<TooN::Vector<3> > _rectangle;

    void compute_rectangle();
};

#endif /* CAPTURE_H */
