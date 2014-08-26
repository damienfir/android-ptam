#include "Capture.h"

#include <android/log.h>

using namespace std;
using namespace TooN;


Capture::Capture()
{
}

Capture::~Capture()
{
}

bool Capture::store_corner(SE3<> c) {
    if (_corners.size() < 3) {
        _corners.push_back(c.get_translation());
        _corners_matrices.push_back(c);
    }
    if (_corners.size() >= 3) {
        compute_rectangle();
        return true;
    }
    return false;
}


void Capture::compute_rectangle()
{
    Vector<3> center = _corners[0] +  (_corners[2] - _corners[0])/2.0;
    Vector<3> v = _corners[1] - center;
    _corners.push_back(center - v);
}

vector<SE3<> > Capture::get_corners_matrices()
{
    return _corners_matrices;
}

vector<Vector<3> > Capture::get_rectangle()
{
    return _corners;
}


void Capture::store_position(SE3<> m)
{
    _positions.push_back(m.get_translation());
}

vector<Vector<3> > Capture::get_positions()
{
    return _positions;
}
