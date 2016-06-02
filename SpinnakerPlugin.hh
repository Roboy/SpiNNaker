#ifndef _GAZEBO_SPINNAKER_HH_
#define _GAZEBO_SPINNAKER_HH_

#include "gazebo/common/Plugin.hh"
#include "gazebo/util/system.hh"
#include <sstream>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>
#include <gazebo/common/common.hh>
#include <boost/bind.hpp>
#include <math.h>
#include <map>
#include <stdio.h>
#include <algorithm> 
#include <boost/numeric/odeint.hpp>



namespace gazebo
{
  
	using namespace std;

	class SpinnakerPlugin : public ModelPlugin
	{
  
	    public: 
	    SpinnakerPlugin();
	    void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);
	    void Init();

	    private: 
	    void OnUpdate();

	   	event::ConnectionPtr connection;
	    common::Time prevUpdateTime;
	    physics::ModelPtr model;
	    std::vector<physics::JointPtr> joints;
	    std::vector<physics::LinkPtr> links;

  };
}

#endif