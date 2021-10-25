#ifndef DAMAGE_H
#define DAMAGE_H

#include <vector>

/**
 * @brief The Damage struct provides a cleaner way for a weapon to deliver multiple effects. e.g. both damage and a torque, or damage to specific sub-systems.
 */
struct Damage {
    // TODO: generalize this later
    // TODO: add shield leach
    float normal_damage;        // Go through shield, armor and finally hull and subsystems
    float phase_damage;         // Bypass shields
    float propulsion_damage;    // Disables the ship's drive
    //float blast_effect;         // Add torque to the ship

    bool Spent() {
        return (normal_damage == 0 && phase_damage == 0 && propulsion_damage == 0);// && blast_effect == 0);
    }

    Damage(float normal_damage = 0,
           float phase_damage = 0,
           float propulsion_damage = 0,
           float blast_effect = 0) :
        normal_damage(normal_damage),
        phase_damage(phase_damage),
        propulsion_damage(propulsion_damage) {}
        //blast_effect(blast_effect),
};

struct InflictedDamage {
    float total_damage;         // Total inflicted damage
    float normal_damage;        // Go through shield, armor and finally hull and subsystems
    float phase_damage;         // Bypass shields
    float propulsion_damage;    // Disables the ship's drive

    // This array stores the damage inflicted to each layer
    // By default, it is  hard-coded to three layers:
    // shield, armor and hull. But this makes this implementation inflexible.
    std::vector<float> inflicted_damage_by_layer;

    InflictedDamage(int number_of_layers = 3) {
        total_damage = 0.0;
        normal_damage = 0.0;
        phase_damage = 0.0;
        propulsion_damage = 0.0;


        for(int i=0;i<number_of_layers;i++) {
            inflicted_damage_by_layer.push_back(0.0f);
        }
    }

    InflictedDamage() = delete;
};

#endif // DAMAGE_H
