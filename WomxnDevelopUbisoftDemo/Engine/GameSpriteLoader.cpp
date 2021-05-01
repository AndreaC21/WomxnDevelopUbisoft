#include "stdafx.h"
#include "GameSpriteLoader.h"

sf::Texture GameSpriteLoader::TextureEnnemy;
sf::Texture GameSpriteLoader::TextureWeapon;
sf::Texture GameSpriteLoader::TexturePortal;

GameSpriteLoader::GameSpriteLoader()
{
	TextureEnnemy.loadFromFile(".\\Assets\\Ennemy\\Idle_1.png");
	TextureWeapon.loadFromFile(".\\Assets\\Weapon\\kunai.png");
	TexturePortal.loadFromFile(".\\Assets\\Portal\\Idle_1.png");
}
