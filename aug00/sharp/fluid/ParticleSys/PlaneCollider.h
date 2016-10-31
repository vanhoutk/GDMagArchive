#ifndef PLANECOLLIDER_H
#define PLANECOLLIDER_H

//
// PlaneCollider
//
// PlaneCollider will collide points against an infinite plane, exerting a penalty force to keep them
// from penetrating it from the front side (i.e. whichever way the normal points.)
//

#include <fluid/ParticleSys/ParticleFunctor.h>
#include <math/Vector.h>

class PlaneCollider : public ParticleFunctor
{
public:
  // This constructor takes the plane equation as the vector (a,b,c) and the scalar d, where the plane
  // equation is ax+by+cz+d = 0.  Note that it's NOT ax+by+cz = d.
  //
  // The third argument is a threshold value, that is, the distance from the plane at which we start
  // repelling the points.  The fourth is the maximum force to exert upon the points (it's a spring, but
  // you need to cap it somewhere.)
  PlaneCollider(Vector abc, float d, float distThresh, float maxRepel);

  virtual void update(PotentialPoints&, float timePassed);
  virtual ParticleFunctorType getType() { return COLLIDER; }

protected:

  Vector abc;
  float d;
  float maxDist;
  float maxForce;
};

#endif //PLANECOLLIDER_H