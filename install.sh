#!/usr/bin/env bash

sudo pacman -S gcc cmake

sudo pacman -Syu

sudo mkdir /artex
cd /artex
sudo mkdir gitsave
sudo mkdir localfiles
sudo mkdir localsaves
sudo mkdir lastBackup
sudo touch versions.txt

cd gitsave
mkdir config

sudo git init
sudo git config --global user.email "Artexautogitroot@gmail.com"
sudo git config --global user.name "Artexautogitroot"

sudo cp ~/Artex/basefiles/* /artex/localfiles

cd ~/Artex

g++ -std=c++17 main.cpp -I include -o Artex
mv Artex /artex/Artex

sudo ln -s ~/Artex/Artex /usr/local/bin/Artex