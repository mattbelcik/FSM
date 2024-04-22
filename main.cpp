/*
 * File:   main.cpp
 * Author: John Sullins
 * Student: Matthew Belcik
 * Description: This program simulates interactions with a character named Allen on the moon.
 * It utilizes a state pattern to manage Allen's mood states and respond to user inputs with mood-appropriate behaviors.
 */
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

class State; // Forward declaration of the State class.

// Manages the current state of Allen and facilitates state transitions.
class StateManager
{
public:
    static State *currentState; // Points to the current state of Allen.
    static map<int,
               State *>
        states; // Maps state IDs to their corresponding State objects.

    // Transitions Allen to a new state based on the newState parameter.
    static void updateAndCheckTransitions(int newState);
};

// Abstract base class representing a state of Allen's mood.
class State
{
public:
    // Method to be called when entering a state. Sets up the state environment.
    virtual void enter() = 0;
    // Method to be called when exiting a state. Handles cleanup.
    virtual void exit() = 0;
    // Updates Allen's mood based on the player's interaction and checks for state transitions.
    virtual void updateMood(int playerApproach) = 0;
    // Outputs Allen's mood to give feedback to the player.
    virtual void expressMood() = 0;

    static int moodLevel; // Tracks Allen's mood, ranging from -2 (very sad) to +2 (very happy).
};

// Initialize static variables
State *StateManager::currentState = nullptr;
map<int, State *> StateManager::states;
int State::moodLevel = 0;

// Implementation of transitioning to a new state.
void StateManager::updateAndCheckTransitions(int newState)
{
    if (currentState != nullptr)
    {
        currentState->exit(); // Cleanup before leaving the current state.
    }
    currentState = states[newState]; // Change to the new state.
    currentState->enter();           // Initialize the new state.
}

// Represents Allen in a happy mood state.
class HappyState : public State
{
public:
    void enter() override
    {
        // Entry behavior for the HappyState.
        cout << "A warm, genuine smile lights up Allen's face as he greets you. His eyes sparkle with an infectious joy, reflecting a sense of contentment and well-being." << endl;
    }

    void exit() override {}

    void updateMood(int playerApproach) override
    {
        // Handles transitions from Happy to other states based on player input.
        switch (playerApproach)
        {
        case 2:                                         // Aggressive
            StateManager::updateAndCheckTransitions(1); // Direct transition to NeutralState
            return;
        case 3:                                         // Confused
            StateManager::updateAndCheckTransitions(3); // Direct transition to ConfusedState
            return;
        case 4:                                         // Sad
            StateManager::updateAndCheckTransitions(4); // Direct transition to SadState
            return;
        case 5:                                         // Fearful
            StateManager::updateAndCheckTransitions(5); // Direct transition to FearfulState
            return;
            // No change for Friendly; Allen remains in HappyState
            // Note: Friendly inputs keep Allen in the HappyState, reinforcing positive interactions.
        }
    }

    void expressMood() override
    {
        cout << "Allen hums a cheerful tune, his steps light and carefree. 'Life's full of wonders, don't you think?' he says with a beaming smile, inviting you to share in his joy." << endl;
    }
};

// Represents Allen in a neutral mood state.
class NeutralState : public State
{
public:
    void enter() override
    {
        // Entry behavior for the NeutralState.
        cout << "Allen takes a deep breath, steadying himself as he assesses his surroundings with a thoughtful gaze." << endl;
    }

    void exit() override {}

    void updateMood(int playerApproach) override
    {
        // Adding a random chance for Allen to find an artifact
        // When neutral allen is looking around and may find an artifact that will instantly make him happy
        int findArtifactChance = rand() % 50;

        if (findArtifactChance < 1)
        { // Adjust the condition based on the rarity you choose
            cout << "Suddenly, Allen stumbles upon a mysterious lunar artifact, sparking joy and excitement." << endl;
            StateManager::updateAndCheckTransitions(0); // Transition to HappyState
            return;                                     // Skip further processing since we're changing the state
        }

        // Direct transition to specific states based on playerApproach
        switch (playerApproach)
        {
        case 1:                                         // Friendly
            StateManager::updateAndCheckTransitions(0); // Direct transition to HappyState
            return;
        case 2:                                         // Aggressive
            StateManager::updateAndCheckTransitions(2); // Direct transition to AngryState
            return;
        case 3:                                         // Confused
            StateManager::updateAndCheckTransitions(3); // Direct transition to ConfusedState
            return;
        case 4:                                         // Sad
            StateManager::updateAndCheckTransitions(4); // Direct transition to SadState
            return;
        case 5:                                         // Fearful
            StateManager::updateAndCheckTransitions(5); // Direct transition to FearfulState
            return;
        }
    }

    void expressMood() override
    {
        cout << "Allen appears contemplative, responding to your presence with a measured curiosity. 'What's next?' he seems to ponder, neither anxious nor overly joyous." << endl;
    }
};

// Allen becomes stuck in angry state, unless the user is friendly with him
class AngryState : public State
{
public:
    void enter() override
    {
        cout << "Allen's demeanor shifts abruptly, his brows furrowing and his jaw setting firm. A stormy expression takes over, signaling a brewing tempest of anger." << endl;
    }

    void exit() override {}

    void updateMood(int playerApproach) override
    {
        int chance = rand() % 10;

        // Check if the random number is 0 (a 10% chance)
        if (chance == 0)
        {
            cout << "Allen has had enough! He pulls out a ray gun and shoots you. Game over." << endl;
            ::exit(0); // Terminate the program
        }
        switch (playerApproach)
        {
        case 1:                                         // Friendly
            StateManager::updateAndCheckTransitions(1); // Direct transition to NeutralState
            return;
            // Implement other cases if specific behavior is desired for AngryState
        }
    }

    void expressMood() override
    {
        cout << "With each word, Allen's voice grows sharper, his frustration palpable. 'Why does it have to be this way?' he demands, struggling to keep his composure." << endl;
    }
};

class ConfusedState : public State
{
public:
    void enter() override
    {
        cout << "Allen pauses, a look of perplexity crossing his features. He scratches his head, clearly puzzled by the situation at hand." << endl;
    }

    void exit() override {}

    void updateMood(int playerApproach) override
    {
        switch (playerApproach)
        {
        case 1:                                         // Friendly
            StateManager::updateAndCheckTransitions(0); // Direct transition to HappyState
            return;
        case 2:                                         // Aggressive
            StateManager::updateAndCheckTransitions(2); // Direct transition to AngryState
            return;
        case 4:                                         // Sad
            StateManager::updateAndCheckTransitions(4); // Direct transition to SadState
            return;
        case 5:                                         // Fearful
            StateManager::updateAndCheckTransitions(5); // Direct transition to FearfulState
            return;
            // Confused approach keeps in ConfusedState, adjust as needed
        }

        // No moodLevel adjustment needed here as direct transitions are handled
    }

    void expressMood() override
    {
        cout << "'I'm not quite sure what to make of this,' Allen admits, his confusion evident as he tries to piece together the puzzle before him." << endl;
    }
};

class SadState : public State
{
public:
    void enter() override
    {
        cout << "A shadow falls over Allen's demeanor, his shoulders slumping slightly as he lets out a deep, wistful sigh. His eyes, once bright, now carry a hint of sorrow." << endl;
    }

    void exit() override {}

    void updateMood(int playerApproach) override
    {
        /*
                if (playerApproach == 1)
                {                   // Friendly
                    moodLevel += 2; // Kindness can uplift Allen's mood
                }
                else if (playerApproach == 2)
                {
                    moodLevel -= 3;
                }
                */
        // testing between using switch statements and if statements
        if (playerApproach == 5)
        {
            StateManager::updateAndCheckTransitions(5);
            return;
        }
        else if (playerApproach == 4)
        {
            StateManager::updateAndCheckTransitions(4);
            return;
        }
        else if (playerApproach == 3)
        {
            StateManager::updateAndCheckTransitions(3);
            return;
        }
        else if (playerApproach == 2)
        {
            StateManager::updateAndCheckTransitions(2);
            return;
        }
        else if (playerApproach == 1)
        {
            StateManager::updateAndCheckTransitions(1);
            return;
        }

        if (moodLevel > 2)
        {
            StateManager::updateAndCheckTransitions(0); // HappyState
        }
        else if (moodLevel > -2)
        {
            StateManager::updateAndCheckTransitions(1); // NeutralState
        }
        else
        {
            // Stays in SadState
        }
    }

    void expressMood() override
    {
        cout << "Allen's voice is soft, tinged with melancholy. 'Sometimes, I just feel a bit lost,' he confides, looking away to hide the vulnerability in his gaze." << endl;
    }
};

class FearfulState : public State
{
public:
    void enter() override
    {
        cout << "Allen's eyes dart around nervously, a visible tension in his posture. He seems on edge, as if expecting something unsettling at any moment." << endl;
    }

    void exit() override {}

    void updateMood(int playerApproach) override
    {
        if (playerApproach == 1)
        {                   // Friendly
            moodLevel += 2; // Friendly actions can help alleviate fear
        }
        /* else if (playerApproach == 2)
         {                   // Aggressive
             moodLevel -= 3; // Threatening actions can intensify fear
         }
         */
        else if (playerApproach == 4)
        {
            StateManager::updateAndCheckTransitions(4);
            return;
        }
        else if (playerApproach == 6)
        {
            StateManager::updateAndCheckTransitions(1);
            return;
        }
        else if (playerApproach == 3)
        {
            StateManager::updateAndCheckTransitions(3);
            return;
        }
        else if (playerApproach == 2)
        {
            StateManager::updateAndCheckTransitions(2);
            return;
        }

        if (moodLevel > 0)
        {
            StateManager::updateAndCheckTransitions(1); // NeutralState, assuming fear is mitigated enough
        }
        else if (moodLevel > -2)
        {
            // Might transition to SadState or stay in FearfulState depending on extended logic
        }
        else
        {
            // Stays in FearfulState, or could transition to AngryState if i add logic for it
        }
    }

    void expressMood() override
    {
        cout << "Allen's movements are jittery, a clear indication of his unease. 'It's hard to shake this feeling,' he murmurs, glancing around as if expecting something to emerge from the shadows." << endl;
    }
};

// Initialization function
void InitializeStates()
{
    // Allocates and initializes each state, associating them with their respective IDs.
    StateManager::states[0] = new HappyState();
    StateManager::states[1] = new NeutralState();
    StateManager::states[2] = new AngryState();
    StateManager::states[3] = new ConfusedState();
    StateManager::states[4] = new SadState();
    StateManager::states[5] = new FearfulState();

    StateManager::updateAndCheckTransitions(1); // Start in NeutralState
}

// Cleanup function
void CleanupStates()
{
    for (auto &state : StateManager::states)
    {
        delete state.second;
    }
    StateManager::states.clear();
}

int main()
{
    InitializeStates();                        // Sets up the states and transitions for the FSM.
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    // Define interaction prompts for different moods
    vector<string> friendlyPrompts = {
        "Share tales of Earth.",
        "Offer a glowing moon rock.",
        "Admire the moon's landscape together.",
        "Show a picture from your travels."};
    vector<string> aggressivePrompts = {
        "Challenge Allen's knowledge of the moon.",
        "Mock Allen for being stuck on this moon.",
        "Ignore Allen when he tries to communicate.",
        "Make a loud noise to startle Allen."};
    vector<string> confusedPrompts = {
        "Speak in a language Allen doesn't understand.",
        "Give Allen an object he's never seen before.",
        "Ask Allen a complex question about human culture.",
        "Explain something using a lot of technical jargon."};
    vector<string> sadPrompts = {
        "Tell Allen about the destruction of a beautiful part of Earth.",
        "Share a personal story of loss and grief.",
        "Describe the feeling of missing a loved one.",
        "Recall a sad moment from your own past."};
    vector<string> fearfulPrompts = {
        "Suddenly approach Allen from behind.",
        "Show Allen a weapon, even if you don't intend to use it.",
        "Describe a dangerous predator from Earth in vivid detail.",
        "Recount a story of a close encounter with danger."};
    vector<string> neutralPrompts = {
        "You kick moondust lightly, watching it float away in the low gravity.",
        "You silently observe Allen, focusing on his expression as he looks out into the void.",
        "You take a moment to gaze at the vast sea of stars, lost in the cosmic spectacle.",
        "You trace the outline of a distant planet with your finger, pondering its mysteries.",
    };

    while (true)
    {
        cout << "+======================================================================================================================================================================+" << endl;
        cout << "\nYou encounter Allen on the alien moon. What do you do?" << endl;
        // Randomly display one prompt from each category
        cout << "1: " << friendlyPrompts[rand() % friendlyPrompts.size()] << " (friendly)" << endl;
        cout << "2: " << aggressivePrompts[rand() % aggressivePrompts.size()] << " (angry)" << endl;
        cout << "3: " << confusedPrompts[rand() % confusedPrompts.size()] << " (confused)" << endl;
        cout << "4: " << sadPrompts[rand() % sadPrompts.size()] << " (sad)" << endl;
        cout << "5: " << fearfulPrompts[rand() % fearfulPrompts.size()] << " (fearful)" << endl;
        cout << "6: " << neutralPrompts[rand() % neutralPrompts.size()] << " (neutral)" << endl;
        cout << "7: Leave Allen in peace and move away." << endl; // Exit option
        int choice;
        cout << "Choose an option (1-7): ";
        cin >> choice;
        cout << "\n";
        cout << "+======================================================================================================================================================================+" << endl;
        // Handle the choice
        if (choice == 7)
        {
            cout << "You decide to leave Allen in peace and continue your exploration of the alien moon." << endl;
            break; // Exiting the loop
        }

        // Updates Allen's mood based on the player's choice and triggers state transition if applicable.
        switch (choice)
        {
        case 1:                                        // Friendly
            StateManager::currentState->updateMood(1); // Friendly approach
            break;
        case 2:                                        // Aggressive
            StateManager::currentState->updateMood(2); // Aggressive approach
            break;
        case 3:                                        // Confused
            StateManager::currentState->updateMood(3); // Confused approach
            break;
        case 4:                                        // Sad
            StateManager::currentState->updateMood(4); // Sad approach
            break;
        case 5:                                        // Fearful
            StateManager::currentState->updateMood(5); // Fearful approach
            break;
        case 6:                                        // Neutral
            StateManager::currentState->updateMood(6); // Neutral approach
            break;

        default:
            cout << "Invalid choice. Try again." << endl;
            continue;
        }

        // After updating the mood, expresses Allen's current mood to the player.
        StateManager::currentState->expressMood();
    }

    CleanupStates();

    return 0;
}