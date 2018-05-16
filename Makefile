# 
#   Copyright 2016 Peter Dunshee <peter@petezah.com>
#
#	This file is part of AvrKeyboardToy.
#
#	AvrKeyboardToy is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	(at your option) any later version.
#
#	AvrKeyboardToy is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with AvrKeyboardToy.  If not, see <http://www.gnu.org/licenses/>.

all: simulator firmware

firmware: Firmware/AvrKeyboardToy.hex
	cd Firmware; make

simulator: simkeytoy
	cd Simulator; make

clean: cleanfirmware cleansimulator

cleanfirmware:
	cd Firmware; make clean

cleansimulator:
	cd Simulator; make clean
