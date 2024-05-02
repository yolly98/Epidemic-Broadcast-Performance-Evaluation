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


network DynamicRadius
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

        @display("bgb=850,500");
    submodules:
        user0: User {
            @display("p=679,167");
        }
        user1: User {
            @display("p=141,48");
        }
        user2: User {
            @display("p=92,190");
        }
        user3: User {
            @display("p=263,119");
        }
        user4: User {
            @display("p=302,246");
        }
        user5: User {
            @display("p=408,167");
        }
        user6: User {
            @display("p=375,386");
        }
        user7: User {
            @display("p=495,256");
        }
        user8: User {
            @display("p=529,384");
        }
        user9: User {
            @display("p=558,111");
        }
        user10: User {
            @display("p=662,37");
        }
        user11: User {
            @display("p=33,87");
        }
}
