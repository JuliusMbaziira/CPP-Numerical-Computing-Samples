#include "DPM.h"

int main () {
	//define an instance of the DPM solver
	DPM coll2p4w;
	//set global parameters
	coll2p4w.k = 10000;
	coll2p4w.gam = 0.4;
	coll2p4w.dt = 1e-4;
	coll2p4w.tMax = 2.0;
	coll2p4w.gravity = {0,0,-9.8};
	//initialize particle with radius, mass, position, velocity
	coll2p4w.particles.push_back({0.05,0.5,{0.3,0.5,0.5},{2,0,0}});
	coll2p4w.particles.push_back({0.05,0.5,{0.7,0.5,0.5},{-1,0,0}});
	//initialize wall with normal and position
	coll2p4w.walls.push_back({{0,0,-1},{0,0,0}});
	coll2p4w.walls.push_back({{0,0, 1},{0,0,1}});
	coll2p4w.walls.push_back({{-1,0,0},{0,0,0}});
	coll2p4w.walls.push_back({{ 1,0,0},{1,0,0}});
	//start the dpm solver
	coll2p4w.solve();
	return 0;
}
