#pragma once
#include <string>
#include <vector>

struct attack {	
	std::string name = "None";
	unsigned int damage = 0;
	std::string desc = "None";
};

struct pokemon {
	std::string name = "None";
	unsigned int level = 1;
	int health = 100;

	std::vector<attack> attacks;

	void add_attack(std::string name, unsigned int damage, std::string desc = "None")
	{	
		attack new_attack = { name, damage, desc };
		attacks.push_back(new_attack);
	}

	void edit_attack(std::string name, unsigned int damage = 0, std::string desc = "None") {
		if (!attacks.empty()) {
			if (damage != 0) {
				for (attack& damage_dealer : attacks) {
					if (damage_dealer.name == name) {
						damage_dealer.damage = damage; 
						break;
					}
				}
			}
			else if (desc != "None") {
				for (attack& damage_dealer : attacks) {
					if (damage_dealer.name == name) {
						damage_dealer.desc = desc;
						break;
					}
				}
			}
		}
	}

	void remove_attack(std::string name) {
		if (!attacks.empty()) {
			for (auto it = attacks.begin(); it != attacks.end(); ++it) {
				if (it->name == name) {
					attacks.erase(it);
					break;
				}
			}
		}
	}
};