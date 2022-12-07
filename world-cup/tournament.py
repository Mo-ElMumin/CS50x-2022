# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():
    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []

    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        # returns a dict with each value from that row,
        # attached to a key, corrispinding with top row
        # e.g.     teams, rating
        #          italy, 20000
        #          spain, 15000
        # First row retuerns this:
        #      = { "team" : "italy",
        #          "rating" : "2000" }
        # second row retuerns this:
        #      = { "team" : "spain",
        #          "rating" : "1500" }
        # notice that the rating is a string, not a integer value,
        # must be type to int to be used in calculations
        # //////
        # for each row
        for row in reader:
            # change the rating values to a int
            row["rating"] = int(row["rating"])
            # add to teams
            teams.append(row)

    counts = {}
    for i in range(N):
        winner = simulate_tournament(teams)
        # returns the name of winning team
        # is the name of the winner, is a preexisting key in the count dict,
        if winner in counts:
            # update the value of key Winner + 1
            counts[winner] += 1
        else:
            # add a pair to the counts dictionary
            # with the key = name of winner
            # and a value of 1
            counts[winner] = 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    while len(teams) > 1:
        teams = simulate_round(teams)
    # return the first element from teams, which is a dictionary
    # return from that dictionary, the value thats paired to the key "team"
    # which returns the name, as a string, of the winning team
    return teams[0]["team"]


if __name__ == "__main__":
    main()
