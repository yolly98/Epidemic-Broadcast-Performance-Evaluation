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

package projectgroup5.simulations;

import projectgroup5.User;


network DynamicFloorplan
{
    parameters:
        int maxWidth = default(100);
        int maxHeight = default(100);
        int N;
        
        @signal[newNode](type=long);
        @signal[activationTime](type=simtime_t);
        @signal[newCollision](type=long);
        @signal[numConnections](type=long);
        @signal[deactivatedUser](type=long);
        @signal[setTau](type=long);
        @statistic[coveredNodes](source="newNode"; record=sum;);
        @statistic[broadcastTime](source="activationTime"; record=max;);
        @statistic[numCollisions](source="newCollision"; record=sum;);
        @statistic[totalConnections](source="numConnections"; record=sum;);
        @statistic[totalInactiveUsers](source="deactivatedUser"; record=sum;);
        @statistic[generatedTau](source="setTau"; record=vector;);
        
    submodules:
        user: User {
            @display("p=130,74");
        }
}
