// ----------------------------------------------------------------------------
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2005, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------
//
//
// Obstacles for use with obstacle avoidance
//
// 10-04-04 bk:  put everything into the OpenSteer namespace
// 09-05-02 cwr: created
//
//
// ----------------------------------------------------------------------------


#ifndef OPENSTEER_OBSTACLE_H
#define OPENSTEER_OBSTACLE_H

#include "Vec3.h"
#include "AbstractVehicle.h"

namespace OpenSteer {
    
    // ----------------------------------------------------------------------------
    // AbstractObstacle: a pure virtual base class for an abstract shape in
    // space, to be used with obstacle avoidance.  (Oops, its not "pure" since
    // I added a concrete method to PathIntersection 11-04-04 -cwr).

    class AbstractObstacle
    {
    public:

        virtual ~AbstractObstacle() {}
        
        // compute steering for a vehicle to avoid this obstacle, if needed
        virtual Vec3 steerToAvoid (const AbstractVehicle& v,
                                   const float minTimeToCollision) const = 0;

        // PathIntersection object: used internally to analyze and store
        // information about intersections of vehicle paths and obstacles.
        class PathIntersection
        {
        public:
            bool intersect; // was an intersection found?
            float distance; // how far was intersection point from vehicle?
            Vec3 surfacePoint; // position of intersection
            Vec3 surfaceNormal; // unit normal at point of intersection
            Vec3 steerHint; // where to steer away from intersection
            bool vehicleOutside; // is the vehicle outside the obstacle?
            const AbstractObstacle* obstacle; // obstacle the path intersects

            // determine steering based on path intersection tests
            Vec3 steerToAvoidIfNeeded (const AbstractVehicle& vehicle,
                                       const float minTimeToCollision) const;

        };

        // find first intersection of a vehicle's path with this obstacle
        // (this must be specialized for each new obstacle shape class)
        virtual void
        findIntersectionWithVehiclePath (const AbstractVehicle& vehicle,
                                         PathIntersection& pi)
            const
            = 0 ;

        // seenFrom (eversion): does this obstacle contrain vehicle to stay
        // inside it or outside it (or both)?  "Inside" describes a clear space
        // within a solid (for example, the interior of a room inside its
        // walls). "Ouitside" describes a solid chunk in the midst of clear
        // space.
        enum seenFromState {outside, inside, both};
        virtual seenFromState seenFrom (void) const = 0;
        virtual void setSeenFrom (seenFromState s) = 0;
    };


    // an STL vector of AbstractObstacle pointers and an iterator for it:
    typedef std::vector<AbstractObstacle*> ObstacleGroup;
    typedef ObstacleGroup::const_iterator ObstacleIterator;


    // ----------------------------------------------------------------------------
    // Obstacle is a utility base class providing some shared functionality

    class Obstacle : public AbstractObstacle
    {
    public:

        Obstacle (void) : _seenFrom (outside) {}

        virtual ~Obstacle() {}
        
        // compute steering for a vehicle to avoid this obstacle, if needed 
        Vec3 steerToAvoid (const AbstractVehicle& v,
                           const float minTimeToCollision)
            const;

        // static method to apply steerToAvoid to nearest obstacle in an
        // ObstacleGroup
        static Vec3 steerToAvoidObstacles (const AbstractVehicle& vehicle,
                                           const float minTimeToCollision,
                                           const ObstacleGroup& obstacles);

        // static method to find first vehicle path intersection in an
        // ObstacleGroup
        static void
        firstPathIntersectionWithObstacleGroup (const AbstractVehicle& vehicle,
                                                const ObstacleGroup& obstacles,
                                                PathIntersection& nearest,
                                                PathIntersection& next);

        seenFromState seenFrom (void) const {return _seenFrom;}

        void setSeenFrom (seenFromState s) {_seenFrom = s;}

    private:
        seenFromState _seenFrom;
    };

} // namespace OpenSteer
    
    
// ----------------------------------------------------------------------------
#endif // OPENSTEER_OBSTACLE_H
