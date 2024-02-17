% CONTROLADOR DO PROEJTO
clc;
clear;
close all;

% Planta contínua
K = 0.03055;
a = 0.03734;
G = tf([K],[1 a])
% rltool(G)

% Discretização do Controlador Contínuo
C = zpk([-0.149],[0],-1.8258)
ts = 51.3; % Tempo de acomodação de 5% em Malha Fechada
T = ts/10; % Período de amostragem

Cd = c2d(C,T,'tustin')


