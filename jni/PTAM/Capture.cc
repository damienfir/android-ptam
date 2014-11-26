#include "../PTAM/Capture.h"

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
    float factor = 2.f;
    Vector<3> center = _corners[0] +  (_corners[2] - _corners[0])/2.0;
    _rectangle.push_back(center + (_corners[0] - center) * factor);
    _rectangle.push_back(center + (_corners[1] - center) * factor);
    _rectangle.push_back(center + (_corners[2] - center) * factor);
    _rectangle.push_back(center - (_corners[1] - center) * factor);
}

vector<SE3<> > Capture::get_corners_matrices()
{
    return _corners_matrices;
}

vector<Vector<3> > Capture::get_rectangle()
{
    if (_rectangle.size() == 0) {
        return _corners;
    }
    else {
        return _rectangle;
    }
}


void Capture::store_position(SE3<> m)
{
    _positions.push_back(m.get_translation());
}


vector<Vector<3> > Capture::get_positions()
{
    return _positions;
}


void Capture::reset_corners() {
	_corners.clear();
	_rectangle.clear();
	_corners_matrices.clear();
	reset_positions();
}

void Capture::reset_positions()
{
	_positions.clear();
}
