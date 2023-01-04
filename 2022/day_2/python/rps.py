import sys

# outcome_score_matrix[opponent_shape][player_shape] = outcome_score
outcome_score_matrix = [[ 3, 6, 0 ], [ 0, 3, 6 ], [ 6, 0, 3 ]]

# player_shape_score_matrix[opponent_shape][outcome] = player_shape_score
player_shape_score_matrix = [[ 3, 1, 2 ], [ 1, 2, 3 ], [ 2, 3, 1 ]]

def get_index_from_letter(letter):
    if letter == 'A' or letter == 'X':
        return 0
    elif letter == 'B' or letter == 'Y':
        return 1
    elif letter == 'C' or letter == 'Z':
        return 2
    return 0

def get_score_from_player_shape(letter):
    match letter:
        case 'X':
            return 1
        case 'Y':
            return 2
        case 'Z':
            return 3
    return 0

def get_score_from_outcome(letter):
    match letter:
        case 'X':
            return 0
        case 'Y':
            return 3
        case 'Z':
            return 6
    return 0


if not len(sys.argv) == 3:
    print("Usage: python rps.py filename second_column_meaning")
    sys.exit()

filename = sys.argv[1]
second_column_meaning = sys.argv[2]

if not second_column_meaning == 'p' and not second_column_meaning == 'o':
    print("Invalid second_column_meaning passed.  Valid options: 'p' or 'o'")
    print(second_column_meaning)
    sys.exit()

total_score = 0
is_second_column_player_shape = second_column_meaning == 'p'

with open(filename) as file:
    for line in file:
        if is_second_column_player_shape:
            total_score += get_score_from_player_shape(line[2])
            total_score += outcome_score_matrix[get_index_from_letter(line[0])][get_index_from_letter(line[2])]
        else:
            total_score += get_score_from_outcome(line[2])
            total_score += player_shape_score_matrix[get_index_from_letter(line[0])][get_index_from_letter(line[2])]

print(total_score)