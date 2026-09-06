#!/usr/bin/env bash

sudo pacman -Syu nlohmann-json

sudo mkdir /artex
cd /artex
sudo mkdir gitsave
sudo mkdir jsonsaves
sudo mkdir localfiles
sudo mkdir localsaves
sudo mkdir lastBackup
sudo touch versions.txt

cd /artex/gitsave

sudo git init
sudo git config --global user.email "ArtexRecoveryautogit@gmail.com"
sudo git config --global user.name "ArtexRecoveryautogit"

sudo cp ~/artexrecovery/basefiles/* /artex/localfiles

cd ~/artexrecovery

mkdir -p include/nlohmann
curl -L https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp -o include/nlohmann/json.hpp

g++ -std=c++17 main.cpp -I include -o ArtexRecovery
sudo ln -s ~/artexrecovery/ArtexRecovery /usr/local/bin/ArtexRecovery