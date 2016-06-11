clear all;
close all;
clc;

input = im2double(imread('images/01.jpeg'));

b_level = 0.8;
b_input = im2bw(input, b_level);

se      = strel('disk', 5);
c_input = imclose(b_input, se);

cb_input = imclearborder(c_input);

se      = strel('disk', 35);
o_input = imopen(cb_input, se);


figure;

subplot(1, 2, 1);
imshow(input);

subplot(1, 2, 2);
imshow(b_input);
%{%}




s = regionprops(o_input, 'centroid');
centroids = cat(1, s.Centroid);

figure;
imshow(input);
hold on;
plot(centroids(:, 1), centroids(:, 2), 'r*');
hold off;




print('-dbmp256', 'output.jpeg');


%{
% Load image, process it, find edges
I  = rgb2gray( imread('images/01.jpeg') );
J = imfilter(I, fspecial('gaussian', [50 50], 50), 'symmetric');
BW = edge(J, 'canny');


figure;
imshow(J);


%# Perform Hough transform and show matrix
[H,T,R] = hough(BW);
imshow(imadjust(mat2gray(H)), [], 'XData',T, 'YData',R, ...
       'InitialMagnification','fit')
xlabel('\theta (degrees)'), ylabel('\rho')
axis on, axis normal, hold on
colormap(hot), colorbar

%# Detect peaks
P  = houghpeaks(H, 4);
plot(T(P(:,2)), R(P(:,1)), 'gs', 'LineWidth',2);

%# Detect lines and overlay on top of image
lines = houghlines(BW, T, R, P);
figure, imshow(I), hold on
for k = 1:length(lines)
    xy = [lines(k).point1; lines(k).point2];
    plot(xy(:,1), xy(:,2), 'g.-', 'LineWidth',2);
end
hold off
%}