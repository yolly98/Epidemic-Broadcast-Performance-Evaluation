//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package projectgroup5;

//
// TODO auto-generated module
//
simple User
{
    parameters:
        //parametro per la generazione dinamica della rete
     	bool dynamicGeneration = default(false);
     	bool dynamicRadius = default(false);
     	
     	//parametri standard
        bool sendInitialMessage = default(false);
        double radius;
        int T;
        int m;
        double t @unit(s);
        
        //statistiche
        @signal[newNode](type=long);
        @signal[activationTime](type=simtime_t);
        @signal[newCollision](type=long);
        @signal[numConnections](type=long);
        @signal[deactivatedUser](type=long);
        @signal[setTau](type=long);

    gates:
        input in[];
        output out[];
}
