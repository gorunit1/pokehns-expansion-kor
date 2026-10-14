#!/usr/bin/env bash
B=/tmp/claude-0/-home-user-pokehns-expansion-kor/b4daa5bd-6dd3-5c4d-aa95-bc177f04e0e3/scratchpad/b22/b; S=/tmp/claude-0/-home-user-pokehns-expansion-kor/b4daa5bd-6dd3-5c4d-aa95-bc177f04e0e3/scratchpad
for v in HEAD UP10711 UP10773 HNS; do
  cp $B/variants/${v}_forced.c $B/m/src/battle_util.c
  (cd $B/m && GITHUB_ACTION=1 make check BUILD=hns -j3 TESTS='B22' > $B/m_$v.log 2>&1; echo "exit $?" >> $B/m_$v.log)
done
FILES="test/battle/multi_battle_whiteout.c test/battle/ability/illusion.c test/party_menu.c test/battle/move_effect/revival_blessing.c test/battle/ai/ai_multi.c test/battle/ai/ai_twelves.c test/battle/trainer_slides.c test/battle/sleep_clause.c test/battle/test_runner_features.c test/battle/ability/commander.c"
for c in base h; do
  : > $B/${c}_tests.log
  for f in $FILES; do
    (cd $B/$c && GITHUB_ACTION=1 make check BUILD=hns -j3 TESTS="$f" >> $B/${c}_tests.log 2>&1; echo "file $f exit $?" >> $B/${c}_tests.log)
  done
done
cd /home/user/pokehns-expansion-kor
ALLOW_REPO=1 HNS_WORK=$S/hns-sync-work HNS_VERIFY_OUT=$S/hns-sync-work/verify-runs dev_scripts/hns_verify/kortests/run.sh $B/h b22b 3 > $B/kortests_h.log 2>&1; echo "exit $?" >> $B/kortests_h.log
echo done > $B/after.done
