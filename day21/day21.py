import functools

@functools.cache
def play(pos1, pos2, score1=0, score2=0):
  if score2 >= 3: return 0, 1

  wins1, wins2 = 0, 0
  for move, n in (3,1),(4,3),(5,6),(6,7),(7,6),(8,3),(9,1):
      pos1_ = (pos1 + move) % 10 or 10
      w2, w1 = play(pos2, pos1_, score2, score1 + pos1_)
      wins1, wins2 = wins1 + n*w1, wins2 + n*w2
  return wins1, wins2

print(play(4, 8))
