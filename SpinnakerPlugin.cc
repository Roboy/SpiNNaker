#include <sstream>
#include <gazebo/gazebo.hh>
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <ignition/math/Vector3.hh>
#include <boost/bind.hpp>
#include <math.h>
#include <stdio.h>
#include <algorithm> 
#include <boost/numeric/odeint.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

#include "SpinnakerPlugin.hh"


#include "SpynnakerLiveSpikesConnection.h"
#include "sender_interface_forward.h"
#include "sender_interface_backward.h"
#include "receiver_interface.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


using namespace gazebo;

//Register plugin with this simulator
GZ_REGISTER_MODEL_PLUGIN(SpinnakerPlugin);


SpinnakerPlugin::SpinnakerPlugin()
{
}

void SpinnakerPlugin::Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
{
	//Store pointer to the model
	this->model = _parent;

	this->joints = this->model->GetJoints();
	this->links = this->model->GetLinks();
	gzdbg << "Model has " 
		  << this->links.size()
		  << " links"
		  << endl;

	for (physics::Joint_V::const_iterator j = this->model->GetJoints().begin();
                        j != this->model->GetJoints().end(); ++j)
    	if ((*j)->GetType() == 1) (*j)->SetAngle(0, 0);	

	this->connection = event::Events::ConnectWorldUpdateBegin(
         			 boost::bind(&SpinnakerPlugin::OnUpdate, this));

}


void SpinnakerPlugin::Init()
{

	try{
		gzdbg << "Booting up SpiNNaker send-receive callbacks..." << endl;
        // set up basic stuff
        char const* send_label1 = "spike_injector_forward";
        char const* send_label2 = "spike_injector_backward";
        char* send_labels[2] = {(char *) send_label1, (char*) send_label2};
        char const* label1 = "pop_forward";
        char const* label2 = "pop_backward";
        char* receive_labels[2] = {(char *) label1, (char*) label2};
        char const* local_host = NULL;
        SpynnakerLiveSpikesConnection connection =
            SpynnakerLiveSpikesConnection(
                2, receive_labels, 2, send_labels, (char*) local_host, 19999);
        // build the SpikeReceiveCallbackInterface
        pthread_mutex_t count_mutex;
        pthread_mutex_init(&count_mutex, NULL);

        SenderInterfaceForward* sender_callback_forward =
            new SenderInterfaceForward(&count_mutex);
        SenderInterfaceBackward* sender_callback_backward =
            new SenderInterfaceBackward(&count_mutex);

        ReceiverInterface* receiver_callback =
            new ReceiverInterface(&count_mutex);
        // register the callback with the SpynnakerLiveSpikesConnection
        connection.add_receive_callback(
            (char*) label1, receiver_callback);
        connection.add_receive_callback(
            (char*) label2, receiver_callback);

        // register the callback with the SpynnakerLiveSpikesConnection
        connection.add_start_callback((char *) send_label1,
                                      sender_callback_forward);
        connection.add_start_callback((char *) send_label2,
                                      sender_callback_backward);
        gzdbg << "Done" << endl;

        while(true){
            sleep(1);
        }
    }
    catch (char const* msg){
    	gzdbg << "Failed" << endl;
        printf("%s \n", msg);
    }
	
}

void SpinnakerPlugin::OnUpdate()
{
	
	//compute the step time

	common::Time currTime = this->model->GetWorld()->GetSimTime();
    common::Time stepTime = currTime - this->prevUpdateTime;
 	this->prevUpdateTime = currTime;


	if (fmod(currTime.Double(),10) == 0 )
	{
		std::map<std::string, double> joint_position_map;
  		joint_position_map["elbow"] = cos(currTime.Double());
		
		this->model->SetJointPositions(joint_position_map);

	}

 }