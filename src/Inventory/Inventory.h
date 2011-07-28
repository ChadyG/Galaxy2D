/*
	Inventory.h
	Galaxy2D
 
	Created by Chad on 7/25/2011
	Copyright 2011 BlitThis! studios. All rights reserved.
 */

/*
Inventory
	holds equipables
	holds stat boosters
	holds consumable items
	Use equipped item(s)
*/
class Weapon;

class Inventory
{
public:
	Inventory() {}

	Weapon* getWeapons() { return m_weaponList; }

	void addWeapon(Weapon *_weapon);

	void selectPrimary(Weapon *_weapon);
	void selectSecondary(Weapon *_weapon);

	void firePrimary();
	void fireSecondary();
private:
	Weapon *m_weaponList;
};