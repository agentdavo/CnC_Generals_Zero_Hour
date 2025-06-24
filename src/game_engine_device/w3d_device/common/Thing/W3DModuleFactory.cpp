/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: W3DModuleFactory.cpp /////////////////////////////////////////////////////////////////////
// Author: Colin Day, April 2001
// Desc:	 W3D specific module
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "game_engine_device/w3d_device/common/w3dmodulefactory.h"
#include "game_engine_device/w3d_device/game_client/Module/w3ddebrisdraw.h"
#include "game_engine_device/w3d_device/game_client/Module/w3ddefaultdraw.h"
#include "game_engine_device/w3d_device/game_client/Module/w3ddependencymodeldraw.h"
#include "game_engine_device/w3d_device/game_client/Module/w3dmodeldraw.h"
#include "game_engine_device/w3d_device/game_client/Module/w3dlaserdraw.h"
#include "game_engine_device/w3d_device/game_client/Module/w3doverlordtankdraw.h"
#include "game_engine_device/w3d_device/game_client/Module/w3dpolicecardraw.h"
#include "game_engine_device/w3d_device/game_client/Module/w3dprojectilestreamdraw.h"
#include "game_engine_device/w3d_device/game_client/Module/w3dropedraw.h"
#include "w3d_device/game_client/Module/W3DSupplyDraw.h"
#include "w3d_device/game_client/Module/W3DScienceModelDraw.h"
#include "w3d_device/game_client/Module/W3DTankDraw.h"
#include "w3d_device/game_client/Module/W3DTruckDraw.h"
#include "w3d_device/game_client/Module/W3DTankTruckDraw.h"
#include "w3d_device/game_client/Module/W3DTracerDraw.h"

//-------------------------------------------------------------------------------------------------
/** Initialize method */
//-------------------------------------------------------------------------------------------------
void W3DModuleFactory::init( void )
{

	// extending functionality
	ModuleFactory::init();

	// add the specific module templates we need for the draw methods
	addModule( W3DDefaultDraw );
	addModule( W3DDebrisDraw );
	addModule( W3DModelDraw );
	addModule( W3DLaserDraw );
	addModule( W3DOverlordTankDraw );
	addModule( W3DProjectileStreamDraw );
	addModule( W3DPoliceCarDraw );
	addModule( W3DRopeDraw );
	addModule( W3DScienceModelDraw );
	addModule( W3DSupplyDraw );
	addModule( W3DDependencyModelDraw );
	addModule( W3DTankDraw );
	addModule( W3DTruckDraw );
	addModule( W3DTracerDraw );
	addModule( W3DTankTruckDraw );

}  // end init
