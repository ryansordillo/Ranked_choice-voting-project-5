#include <iostream>
using namespace std;

class Ballot {

private:
    int * _ballot;
    int _num_candidates;
public:
    Ballot(){

    }
    /**
     * @param int num_candidates number of candidates
     * @brief reads each ballot and fills in with input
    */
    void read_ballot(int num_candidates) {
        //fill in each ballot
        _num_candidates = num_candidates;
        _ballot = new int[_num_candidates];
        for (int i = 0; i < _num_candidates; i++) {
            cin >> _ballot[i];
        }
    }
    /**
    *@brief eliminates the candidate passed to the method from each ballot, marks them as -1
    */
    void eliminate_candidate(int eliminated_candidate) {
        //iterate through each ballot and eliminate the correct candidate
        for (int i = 0; i < _num_candidates; i++) {
            if (_ballot[i] == eliminated_candidate) {
                _ballot[i] = -1;  // Mark as eliminated
            }
        }
    }
    /**
    *@brief checks if each ballot is valid
    */
    bool is_valid() {
        //check if the ballot is valid, returns false if all candidates are eliminated denoted by -1
        for (int i = 0; i < _num_candidates; i++) {
            if (_ballot[i] != -1) {
                return true;
            }
        }
        return false;
    }
    /**
    *@brief gets the first choice for each ballot
    */
    int get_first_choice() {
        //gets the first choice of each ballot
        for (int i = 0; i < _num_candidates; i++) {
            if (_ballot[i] != -1) {
                return _ballot[i];
            }
        }
        return -1;  // No valid first choice
    }
    /**
    *@brief shifts each ballot over to the left and replaces end with -1
    */
    void shift_ballot() {
        //shifts each ballot to the left if the first pick is eliminated
        if (_ballot[0] == -1) {
            for (int i = 0; i < (_num_candidates - 1); i++) {
                _ballot[i] = _ballot[i + 1];
            }
            //replaces last element with -1
            _ballot[_num_candidates - 1] = -1;
            
        }
    }
    /**
    *@brief deletes ballot
    */
    ~Ballot() {
        delete[] _ballot;
    }
};

class Tally {
private:
    int _num_candidates;
    int _num_ballots;
    Ballot* _vote_sheet;
    int* _first_place_votes;
    int _most;
    int _first_pick;
    Ballot* _b;
    Ballot** _ptrs;
    int _num_most;
    int _num_eliminated;
    int _min;
public:
    /**
    * @param int num_candidates number of candidates
    * @param int num_ballots number of ballots
    *@brief constructs the vote_sheet with ballots from Ballot class
    *gets each ballots first choice and contstructs array for first place votes
    *
    */
    //constructor for the tally class
    Tally(int num_candidates, int num_ballots) {
        _num_candidates = num_candidates;
        _num_ballots = num_ballots;
        _vote_sheet = new Ballot [_num_ballots]; //creates array of ballots
        _first_place_votes = new int[_num_candidates](); //creates array of how many first place votes each candidate has
        _ptrs = new Ballot * [_num_ballots];
        for (int i = 0; i < _num_ballots; i++) { //fills in vote sheet array and does not overwrite data
            _vote_sheet[i].read_ballot(_num_candidates);
            _ptrs[i] = _b;
            int _first_pick = _vote_sheet[i].get_first_choice(); //gets first choice for each ballot
            if (_first_pick != -1) {
                _first_place_votes[_first_pick] += 1;
            }
        }
        most();
    }
    /**
    *@brief updates first place votes array
    */
    void update_first_votes() {
        //replaces all contending candidates first place votes with 0
        for (int i = 0; i < _num_candidates; i++) {
            if (_first_place_votes[i] != -1)
                _first_place_votes[i] = 0;
        }
        //counts each ballots first place votes
        for (int i = 0; i < _num_ballots; i++) {
            _first_pick = _vote_sheet[i].get_first_choice();
            if (_first_pick != -1)
                _first_place_votes[_first_pick]++;
        }
        most();
    }
    /**
    *@brief checks for ties
    *@return true if there is a tie, false if not
    */
    bool check_ties() {
        //checks for ties returns true if there is a tie
        _most = 0;
        _num_most = 0;
        _num_eliminated = 0;
        //gets the value for most votes
        for (int i = 0; i < _num_candidates; i++) {
            if (_first_place_votes[i] > _most) {
                _most = _first_place_votes[i];
            }
        }
        //gets number for how many candidates have most votes and tracks how many are eliminated
        for (int i = 0; i < _num_candidates; i++) {
            if (_first_place_votes[i] == _most) {
                _num_most++;
            }
            else if (_first_place_votes[i] == -1)
                _num_eliminated++;
        }
        //calculation to check for ties
        if (_num_most > 1 && _num_eliminated == (_num_candidates - _num_most))
            return true;
        return false;
    }
    /**
    * @brief gets which candidate has the most first place votes
    */
    void most() {
        //gets the value for the most first place votes by going through first place votes array
        _most = 0;
        for (int i = 0; i < _num_candidates; i++) {

            if (_first_place_votes[i] > _most) {
                _most = _first_place_votes[i];
            }
        }
    }
    /**
     * @brief checks if there is a winner in the election
     * @return true if no winner yet and false if there is a winner
    */
    bool no_winner_yet() {
        //check if there is no winner yet, returns true if no winner
        for (int i = 0; i < _num_candidates; i++) {
            //majority check for odd number ballots
            if (_num_ballots % 2 == 1) {
                //if candidate has majority or there is a tie returns false
                if (_first_place_votes[i] >= ((_num_ballots + 1) / 2) || check_ties()) 
                    return false;
            }
            else {
                if (_first_place_votes[i] > (_num_ballots / 2) || check_ties()) 
                    return false;
            }
        }
        return true;
    }
    /**
    *@brief eliminates candidates who have the least first place votes
    */
    void eliminate_losers() {
        //finds the minimum number of first place votes
        _min = _num_ballots;
        for (int i = 0; i < _num_candidates; i++) {
            if (_first_place_votes[i] < _min && _first_place_votes[i] != -1)
                _min = _first_place_votes[i];
        }
        //goes through each ballot in vote sheet and eliminates that candidate
        for (int i = 0; i < _num_candidates; i++) {
            if (_first_place_votes[i] == _min) {
                for (int j = 0; j < _num_ballots; j++)
                    _vote_sheet[j].eliminate_candidate(i);
                _first_place_votes[i] = -1;
            }
        }
    }
    /**
    *@brief updates the vote sheet after eliminating candidates
    */
    void update_prefs() {
        //calls shift_ballot on each ballot in voter sheet
        for (int i = 0; i < _num_ballots; i++) {
            _vote_sheet[i].shift_ballot();
        }
        //update first place votes array
        update_first_votes();
    }
    /**
    * @brief checks if the election is still valid
    * @return true if still valid, false if otherwise
    */
    bool election_still_valid() {
        //checks if the vote sheet is still valid by calling is_valid from ballot class
        for (int i = 0; i < _num_ballots; i++) {
            if (_vote_sheet[i].is_valid()) {
                return true;
            }
        }
        return false;
    }
    /**
    * @brief gets the winner of the election
    */
    int get_winner() {
        //if tie return -1 to signal inconclusive vote
        if (check_ties())
            return -1;
        for (int i = 0; i < _num_candidates; i++) {
            if (_first_place_votes[i] > _num_ballots / 2 || _first_place_votes[i] == _most) {
                return i;
            }
        }
        return -1; // No winner found
    }
    /**
    *@brief resolves vote by ranked choice voting
    */
    void resolve_vote() {
        //eliminates losers from the voter sheet and each ballot
        //the preferences are updated based on eliminated candidates
        //while there is no winner and election still valid this process continues
        //once winner is found it outputs the winning candidate or election inconclusive if tie
        eliminate_losers();
        update_prefs();
        while (no_winner_yet() && election_still_valid()) {
            // cout << "in while loop " << endl;
            eliminate_losers();
            update_prefs();
        }
        int winner = get_winner();
        if (winner == -1) {
            cout << "The election is inconclusive." << endl;
        } else {
            cout << "The winner is: " << winner << endl;
        }
    }
    /**
    *@brief destructor
    */
    ~Tally() {
        //destructor for tally class
        // for (int i = 0; i < _num_ballots; i++) {
        //     delete ptrs[i];
        // }
        delete[] _ptrs;
        delete[] _vote_sheet;
        delete[] _first_place_votes;
    }
};

int main() {
    int num_candidates = 0;
    cin >> num_candidates;
    int num_ballots = 0;
    cin >> num_ballots;
    Tally tal(num_candidates, num_ballots);
    tal.resolve_vote();
    return 0;
}