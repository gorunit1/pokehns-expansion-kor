#ifndef GUARD_CHOOSEBOXMON_H
#define GUARD_CHOOSEBOXMON_H

struct MoveLearnUI {
    void (*askConfirmation)(void);
    s32 (*waitConfirmation)(void);
    void (*printMessage)(const u8 *message);
    void (*playFanfare)(u32 songId);
    void (*showMoveList)(u8 taskId);
    void (*endTask)(u8 taskId);
    bool8 fanfareAfterReplace; // HnS: also play (and wait for) the fanfare after replacing a move; the move relearner did so before upstream #8491, the field move tutors did not
};

u32 IsBoxMonExcluded(struct BoxPokemon *boxmon);
bool32 CanBoxMonBeSelected(struct BoxPokemon *boxmon);
s32 LearnMove(const struct MoveLearnUI *ui, u8 taskId);
s32 GetLearnMoveStartState(void);
s32 GetLearnMoveStartAfterPromptState(void);
s32 GetLearnMoveResumeAfterSummaryScreenState(void);

#if TESTING
const struct MoveLearnUI *GetMoveRelearnerLearnUI(void);  // src/move_relearner.c
const struct MoveLearnUI *GetFieldMoveTutorLearnUI(void); // src/field_specials.c
#endif

#endif // GUARD_CHOOSEBOXMON_H
