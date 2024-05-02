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

network Floorplan
{
     parameters:
        @signal[newNode](type=long);
        @signal[activationTime](type=simtime_t);
        @signal[newCollision](type=long);
        @statistic[coveredNodes](source="newNode"; record=sum,vector;);
        @statistic[broadcastTime](source="activationTime"; record=max;);
        @statistic[numCollisions](source="newCollision"; record=sum,vector;);
        
    @display("bgb=599,316");
    
    submodules:
        user: User {
            @display("p=73,83");
        }
        user1: User {
            @display("p=247,54");
        }
        user2: User {
            @display("p=119,225");
        }
        user3: User {
            @display("p=199,158");
        }
        user4: User {
            @display("p=354,40");
        }
        user5: User {
            @display("p=444,147");
        }
        user6: User {
            @display("p=532,33");
        }
        user7: User {
            @display("p=323,238");
        }
        
    connections:
        user.out++ --> user1.in++;
        user.out++ --> user2.in++;
        user2.out++ --> user.in++;
        user1.out++ --> user.in++;
        user1.out++ --> user3.in++;
        user3.out++ --> user1.in++;
        user3.out++ --> user2.in++;
        user2.out++ --> user3.in++;
        user3.out++ --> user7.in++;
        user3.out++ --> user4.in++;
        user7.out++ --> user3.in++;
        user4.out++ --> user3.in++;
        user7.out++ --> user5.in++;
        user5.out++ --> user7.in++;
        user7.out++ --> user4.in++;
        user4.out++ --> user7.in++;
        user5.out++ --> user6.in++;
        user6.out++ --> user5.in++;
        
}
