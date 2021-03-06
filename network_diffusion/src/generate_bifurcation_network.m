%======================================================================
%                        "BGLgeom library"
%        Course on Advanced Programming for Scientific Computing
%                      Politecnico di Milano
%                          A.Y. 2015-2016
%                  
%  		 Copyright (C) 2017 Ilaria Speranza & Mattia Tantardini
%======================================================================
   
%   This program is free software: you can redistribute it and/or modify
%   it under the terms of the GNU General Public License as published by
%   the Free Software Foundation, either version 3 of the License, or
%   (at your option) any later version.
%
%   This program is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
%   GNU General Public License for more details.
%
%   You should have received a copy of the GNU General Public License
%   along with this program. If not, see <http://www.gnu.org/licenses/>.

%======================================================================

% Octave script to generate a tree network with the desired order of
% bifurcations. The network bifurcates the first time along the z 
% direction, the second on the x direction, and so on, proceeding 
% along the y direction. It also provide a random effect on the 
% final position of each node, but the first two, that are fixed.
% The network is contained in a 3D cube with unitary side.
% 
% Input parameters:
%	n = number of the order of bifurcation (at least 2 to obtain
%		a Y configuration.
%	re = random effect to apply to the nodes. When the new node 
%		is generated, a draw, multiplied by re, from a uniform 
%		between -1 and 1 is added to each coordinate. We perform
%		checks not to go out of bounds
%	out_filanem = name of the output file. Please, always put it 
%		in the data/ folder. The output only stores a list of 
%		edges: each row displays 'coordinates of source', 
%		'coordinates of target'

close all
clear all

% Number of order of bifurcation
n = 4;
% Setting random effect
re = 0.05;
% Output file name
out_filename = 'data/bifurcation_network.txt'

% Box lenght: unitary
L = 1;
% Initial point:
init = [0.5,0,0.5];

% y step size:
h_y = L/n;
% x step size:
h_x = L/(n-1);
% z step size
h_z = L/(n-2);

% Storing the points: (iteration 0)
points = init;
% First step: linear till the first next point (iteration 1)
new_point = [ init(1), init(2)+h_y, init(3)];
points = [points, new_point];   % I use the notation 'source' 'target'
% Initial boundary condition
BC = [' DIR 1 INT 0'];
% From the second iteration
for i = 2:n
    new_points = [];
    num_old_point_to_bifurcate = 2^(i-2);    
    for j = 0:num_old_point_to_bifurcate-1 %to set the indices in the proper way
    %If it is a an odd iteration, split on plane z
    if mod(i,2) == 0
        new_point1 = [points(end-j,4) - h_x/2 + re*(-1+2*rand(1)), ...
                      points(end-j,5) + h_y + re*(-1+2*rand(1)), ...
                      points(end-j,6) + re*(-1+2*rand(1))];
        new_point2 = [points(end-j,4) + h_x/2 + re*(-1+2*rand(1)), ...
                      points(end-j,5) + h_y + re*(-1+2*rand(1)), ...
                      points(end-j,6) + re*(-1+2*rand(1))];
    end
    % If it is an even iteration, split on plane x
    if mod(i,2) == 1
        new_point1 = [points(end-j,4) + re*(-1+2*rand(1)), ...
                      points(end-j,5) + h_y + re*(-1+2*rand(1)), ....
                      points(end-j,6) - h_z/2 + re*(-1+2*rand(1))];
        new_point2 = [points(end-j,4) + re*(-1+2*rand(1)), ...
                      points(end-j,5) + h_y + re*(-1+2*rand(1)), ...
                      points(end-j,6) + h_z/2 + re*(-1+2*rand(1))];
    end
    % Now we control whether the new points are out of bounds
    for k = 1:3
        if new_point1(1,k) - 1 < -1 %coordinate < 0
            new_point1(1,k) = 0;
        elseif new_point1(1,k) - 1 > 0 %coordinate > 1
            new_point1(1,k) = 1;
        end
        if new_point2(1,k) -1 < -1 %coordinate < 0
            new_point2(1,k) = 0;
        elseif new_point2(1,k) - 1 > 0 %coordinate > 1
            new_point2(1,k) = 1;            
        end
    end
    % Storing temporary points
    new_points = [new_points; points(end-j,4:6), new_point1];
    new_points = [new_points; points(end-j,4:6), new_point2];
    if i ~= n
    	BC = [BC; ' INT 0 INT 0'; ' INT 0 INT 0'];
    else
    	BC = [BC; ' INT 0 MIX 0'; ' INT 0 MIX 0'];	% Outflow condition
    end
    
    end
    % Storing all new points of this iteration
    points = [points; new_points];
end

% Plotting first point
plot3(points(1,1), points(1,2), points(1,3), '*', 'MarkerSize', 10)
% Plotting all other points
hold on
plot3(points(:,4), points(:,5), points(:,6), '*', 'MarkerSize', 10)
% Plotting the edge
for i = 1:size(points,1)
	plot3(points(i,[1,4]), points(i,[2,5]), points(i,[3,6]), '-g', 'LineWidth', 2)
end
grid on
axis equal

%Building final matrix with coordinates and boundary conditions
final = [num2str(points), BC];
% Printing the output on a txt file
dlmwrite(out_filename, final, 'delimiter', '');
