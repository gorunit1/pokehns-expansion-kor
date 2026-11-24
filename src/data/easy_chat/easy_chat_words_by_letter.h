#include "easy_chat.h"

#define DOUBLE_SPECIES_NAME EC_EMPTY_WORD, 2,

const u16 gEasyChatWordsByLetter_KoreanOthers[] = {
    // !
    EC_WORD_EXCL,
    // !!
    EC_WORD_EXCL_EXCL,
    // !?
    EC_WORD_QUES_EXCL,
    // -
    EC_WORD_DASH,
    // ...
    EC_WORD_ELLIPSIS,
    // ...!
    EC_WORD_ELLIPSIS_EXCL,
    // ......
    EC_WORD_ELLIPSIS_ELLIPSIS_ELLIPSIS,
    // ?
    EC_WORD_QUES,
    // - - -
    EC_WORD_DASH_DASH_DASH,
    // 10만볼트
    EC_MOVE(THUNDERBOLT),
    // HP회복
    EC_MOVE(RECOVER),
    // PC
    EC_WORD_PC,
    // RPG
    EC_WORD_RPG,
    // TV
    EC_WORD_TELEVISION,
};

const u16 gEasyChatWordsByLetter_KoreanGiyeok[] = {
    // 가까운
    EC_WORD_NEAR,
    // 가능하다
    EC_WORD_GOING,
    // 가득
    EC_WORD_FULL,
    // 가디
    EC_POKEMON_NATIONAL(GROWLITHE),
    // 가디안
    EC_POKEMON(GARDEVOIR),
    // 가라
    EC_WORD_GO,
    // 가로채기
    EC_MOVE2(SNATCH),
    // 가르친다
    EC_WORD_TEACHES,
    // 가르침
    EC_WORD_TEACH,
    // 가뭄
    EC_WORD_DROUGHT,
    // 가방
    EC_WORD_BAG,
    // 가보리
    EC_POKEMON(ARON),
    // 가속
    EC_WORD_SPEED_BOOST,
    // 가시대포
    EC_MOVE2(SPIKE_CANNON),
    // 가위자르기
    EC_MOVE2(GUILLOTINE),
    // 가을
    EC_WORD_FALL,
    // 가이오가
    EC_POKEMON(KYOGRE),
    // 가자
    EC_WORD_UGLY,
    // 가재군
    EC_POKEMON(CORPHISH),
    // 가재장군
    EC_POKEMON(CRAWDAUNT),
    // 가족
    EC_WORD_FAMILY,
    // 가지고
    EC_WORD_OWN,
    // 각오
    EC_WORD_DECIDE,
    // 각지기
    EC_MOVE(SHARPEN),
    // 간다
    EC_WORD_HERE_GOES,
    // 간지르기
    EC_MOVE(TICKLE),
    // 갈게
    EC_WORD_HERE_I_COME,
    // 갈래
    EC_WORD_GOES,
    // 갈모매
    EC_POKEMON(WINGULL),
    // 감사합니다
    EC_WORD_APPRECIATE,
    // 감상
    EC_WORD_VIEWING,
    // 갑니다
    EC_WORD_HERE_IT_IS,
    // 갔다
    EC_WORD_WENT,
    // 강인한
    EC_WORD_TOUGH,
    // 강인함
    EC_WORD_TOUGHNESS,
    // 강챙이
    EC_POKEMON_NATIONAL(POLIWRATH),
    // 강철
    EC_WORD_STEEL,
    // 강철날개
    EC_MOVE2(STEEL_WING),
    // 강철톤
    EC_POKEMON_NATIONAL(STEELIX),
    // 강하다
    EC_WORD_STRONG,
    // 갖고 싶다
    EC_WORD_WANT,
    // 같은
    EC_WORD_SEE,
    // 같은 느낌
    EC_WORD_DOES,
    // 같이
    EC_WORD_TOGETHER,
    // 개무소
    EC_POKEMON(WURMPLE),
    // 개미지옥
    EC_WORD_ARENA_TRAP,
    // 객기
    EC_MOVE(FACADE),
    // 갱도라
    EC_POKEMON(LAIRON),
    // 갸라도스
    EC_POKEMON(GYARADOS),
    // 갸라도스
    EC_POKEMON_NATIONAL(GYARADOS),
    // 걔
    EC_WORD_THEY,
    // 걔가
    EC_WORD_THEY_WERE,
    // 걔는
    EC_WORD_THEY_RE,
    // 걔를
    EC_WORD_THEY_VE,
    // 걔에게
    EC_WORD_TO_THEM,
    // 걔의
    EC_WORD_THEIR,
    // 거꾸로
    EC_WORD_UPSIDE_DOWN,
    // 거니?
    EC_WORD_BELONGS_TO,
    // 거라구
    EC_WORD_AS,
    // 거란다
    EC_WORD_FOR,
    // 거랍니다
    EC_WORD_DO,
    // 거미집
    EC_MOVE(SPIDER_WEB),
    // 거북왕
    EC_POKEMON_NATIONAL(BLASTOISE),
    // 거야
    EC_WORD_CAN,
    // 거지-!
    EC_WORD_DON_T,
    // 거짓말
    EC_WORD_LIE,
    // 거짓울음
    EC_MOVE(FAKE_TEARS),
    // 거품
    EC_MOVE(BUBBLE),
    // 거품광선
    EC_MOVE2(BUBBLE_BEAM),
    // 건가
    EC_WORD_ON,
    // 건강한
    EC_WORD_HEALTHY,
    // 건데
    EC_WORD_TO,
    // 걷기
    EC_WORD_WALKING,
    // 걷는다
    EC_WORD_WALKS,
    // 검은눈빛
    EC_MOVE(MEAN_LOOK),
    // 겁나는얼굴
    EC_MOVE(SCARY_FACE),
    // 겁니다
    EC_WORD_WITH,
    // 것
    EC_WORD_THING,
    // 것이다
    EC_WORD_OFF,
    // 게으름
    EC_WORD_TRUANT,
    // 게으름피우기
    EC_MOVE2(SLACK_OFF),
    // 게을로
    EC_POKEMON(SLAKOTH),
    // 게을킹
    EC_POKEMON(SLAKING),
    // 게임
    EC_WORD_GAME,
    // 겟
    EC_WORD_GOTCHA,
    // 겨울
    EC_WORD_WINTER,
    // 격투
    EC_WORD_FIGHTING,
    // 결정
    EC_WORD_CHOOSE,
    // 경우
    EC_WORD_CASE,
    // 계속
    EC_WORD_MONTH,
    // 고!
    EC_WORD_ARE,
    // 고드름침
    EC_MOVE2(ICICLE_SPEAR),
    // 고라파덕
    EC_POKEMON(PSYDUCK),
    // 고라파덕
    EC_POKEMON_NATIONAL(PSYDUCK),
    // 고래왕
    EC_POKEMON(WAILORD),
    // 고래왕자
    EC_POKEMON(WAILMER),
    // 고마워
    EC_WORD_THANKS,
    // 고맙습니다
    EC_WORD_MUCH_OBLIGED,
    // 고민
    EC_WORD_WORRY,
    // 고속스핀
    EC_MOVE(RAPID_SPIN),
    // 고속이동
    EC_MOVE(AGILITY),
    // 고스트
    EC_WORD_GHOST,
    // 고양이돈받기
    EC_MOVE2(PAY_DAY),
    // 고오스
    EC_POKEMON_NATIONAL(GASTLY),
    // 고우스트
    EC_POKEMON_NATIONAL(HAUNTER),
    // 고지
    EC_POKEMON(SANDSLASH),
    // 고지
    EC_POKEMON_NATIONAL(SANDSLASH),
    // 곧
    EC_WORD_SOON,
    // 골덕
    EC_POKEMON(GOLDUCK),
    // 골덕
    EC_POKEMON_NATIONAL(GOLDUCK),
    // 골드
    EC_WORD_GOLD,
    // 골뱃
    EC_POKEMON(GOLBAT),
    // 골뱃
    EC_POKEMON_NATIONAL(GOLBAT),
    // 공격
    EC_WORD_ATTACK,
    // 공부
    EC_WORD_STUDY,
    // 공세
    EC_WORD_OFFENSIVE,
    // 공중날기
    EC_MOVE(FLY),
    // 과연
    EC_WORD_UNDERSTOOD,
    // 과자
    EC_WORD_SWEETS,
    // 관전
    EC_WORD_SPECTATOR,
    // 광합성
    EC_MOVE(SYNTHESIS),
    // 괜찮아
    EC_WORD_ALL_RIGHT,
    // 괴력
    EC_MOVE(STRENGTH),
    // 괴력몬
    EC_POKEMON(MACHAMP),
    // 괴력몬
    EC_POKEMON_NATIONAL(MACHAMP),
    // 괴력집게
    EC_WORD_HYPER_CUTTER,
    // 괴로운
    EC_WORD_HARD,
    // 괴로웠다
    EC_WORD_TERRIBLE,
    // 굉장한
    EC_WORD_MUCH,
    // 교환
    EC_WORD_TRADE,
    // 구구
    EC_POKEMON_NATIONAL(PIDGEY),
    // 구르기
    EC_MOVE(ROLLOUT),
    // 구멍파기
    EC_MOVE(DIG),
    // 구슬던지기
    EC_MOVE2(BARRAGE),
    // 군
    EC_WORD_CHILDREN,
    // 귀여운
    EC_WORD_CUTE,
    // 귀여운 척
    EC_WORD_WANNABE,
    // 귀여움
    EC_WORD_CUTENESS,
    // 그
    EC_WORD_HIM,
    // 그거
    EC_WORD_THAT,
    // 그거야
    EC_WORD_THAT_S_IT_EXCL,
    // 그건
    EC_WORD_THOSE_WERE,
    // 그것만
    EC_WORD_AT,
    // 그녀
    EC_WORD_HER,
    // 그라에나
    EC_POKEMON(MIGHTYENA),
    // 그란돈
    EC_POKEMON(GROUDON),
    // 그랑블루
    EC_POKEMON_NATIONAL(GRANBULL),
    // 그래
    EC_WORD_AGREE,
    // 그래도
    EC_WORD_EVEN_SO,
    // 그래서
    EC_WORD_AND,
    // 그러니까
    EC_WORD_THOSE,
    // 그런
    EC_WORD_THAT_WAS,
    // 그럼 안녕
    EC_WORD_SEE_YA,
    // 그럼 이만
    EC_WORD_WELL_THEN,
    // 그렇구나
    EC_WORD_SHAKE,
    // 그룹
    EC_WORD_GROUP,
    // 그린
    EC_WORD_GREEN,
    // 그림자밟기
    EC_WORD_SHADOW_TAG,
    // 그림자분신
    EC_MOVE(DOUBLE_TEAM),
    // 그분
    EC_WORD_SHE,
    // 그 사람
    EC_WORD_WOMAN,
    // 그야
    EC_WORD_NATURAL,
    // 그저께
    EC_WORD_DAYS,
    // 그쪽
    EC_WORD_THOSE_ARE,
    // 그쵸-!
    EC_WORD_NOT,
    // 근사한
    EC_WORD_COOL,
    // 근사함
    EC_WORD_COOLNESS,
    // 근성
    EC_WORD_GUTSY,
    // 근성
    EC_WORD_GUTS,
    // 근육몬
    EC_POKEMON(MACHOKE),
    // 근육몬
    EC_POKEMON_NATIONAL(MACHOKE),
    // 글라이거
    EC_POKEMON_NATIONAL(GLIGAR),
    // 금속음
    EC_MOVE(METAL_SOUND),
    // 금요일
    EC_WORD_FRIDAY,
    // 급류
    EC_WORD_TORRENT,
    // 기가드레인
    EC_MOVE(GIGA_DRAIN),
    // 기념
    EC_WORD_COMMEMORATE,
    // 기묘한
    EC_WORD_FUNNY,
    // 기분
    EC_WORD_FEELING,
    // 기분 탓
    EC_WORD_MISHEARD,
    // 기분파
    EC_WORD_FORECAST,
    // 기쁜
    EC_WORD_HAPPY,
    // 기쁨
    EC_WORD_HAPPINESS,
    // 기사회생
    EC_MOVE(REVERSAL),
    // 기세
    EC_WORD_MOMENTUM,
    // 기술
    EC_WORD_MOVE,
    // 기절
    EC_WORD_FAINT,
    // 기충전
    EC_MOVE(FOCUS_ENERGY),
    // 기합
    EC_WORD_SPIRIT,
    // 길동무
    EC_MOVE2(DESTINY_BOND),
    // 김밥말이
    EC_MOVE2(WRAP),
    // 깃털댄스
    EC_MOVE2(FEATHER_DANCE),
    // 깊은
    EC_WORD_DEEP,
};

const u16 gEasyChatWordsByLetter_KoreanSsangGiyeok[] = {
    // 까지
    EC_WORD_UNTIL,
    // 까칠한피부
    EC_WORD_ROUGH_SKIN,
    // 깜까미
    EC_POKEMON(SABLEYE),
    // 깜지곰
    EC_POKEMON_NATIONAL(TEDDIURSA),
    // 깜짝
    EC_WORD_SURPRISE,
    // 깨갱
    EC_WORD_FLATTEN,
    // 깨뜨리다
    EC_MOVE(BRICK_BREAK),
    // 깨물어부수기
    EC_MOVE(CRUNCH),
    // 깨비드릴조
    EC_POKEMON_NATIONAL(FEAROW),
    // 깨비참
    EC_POKEMON_NATIONAL(SPEAROW),
    // 꺄-
    EC_WORD_EEK,
    // 껄껄껄
    EC_WORD_GWAHAHAHA,
    // 껍질끼우기
    EC_MOVE(CLAMP),
    // 껍질몬
    EC_POKEMON(SHEDINJA),
    // 껍질에숨기
    EC_MOVE(WITHDRAW),
    // 꼬렛
    EC_POKEMON_NATIONAL(RATTATA),
    // 꼬르륵
    EC_WORD_HUNGRY,
    // 꼬리선
    EC_POKEMON_NATIONAL(SENTRET),
    // 꼬리흔들기
    EC_MOVE(TAIL_WHIP),
    // 꼬마돌
    EC_EMPTY_WORD, 2,
    EC_POKEMON(GEODUDE),
    EC_POKEMON_NATIONAL(GEODUDE),
    // 꼬부기
    EC_POKEMON_NATIONAL(SQUIRTLE),
    // 꼬지모
    EC_POKEMON_NATIONAL(SUDOWOODO),
    // 꼴깍몬
    EC_POKEMON(GULPIN),
    // 꽃
    EC_WORD_FLOWERS,
    // 꽃잎댄스
    EC_MOVE2(PETAL_DANCE),
    // 꽈악
    EC_WORD_TIGHT,
    // 꾸꾸리
    EC_POKEMON_NATIONAL(SWINUB),
    // 꿀꺽
    EC_MOVE2(SWALLOW),
    // 꿀꺽꿀꺽
    EC_WORD_THIRSTY,
    // 꿀꺽몬
    EC_POKEMON(SWALOT),
    // 꿈
    EC_WORD_DREAM,
    // 꿈먹기
    EC_MOVE2(DREAM_EATER),
    // 꿰뚫어보기
    EC_MOVE2(FORESIGHT),
    // 끝
    EC_WORD_END,
    // 끝낸다
    EC_WORD_STORES,
    // 낄낄
    EC_WORD_TICKLISH,
};

const u16 gEasyChatWordsByLetter_KoreanNieun[] = {
    // 나
    EC_WORD_I,
    // 나무돌이
    EC_POKEMON(GROVYLE),
    // 나무열매
    EC_WORD_BERRY,
    // 나무지기
    EC_POKEMON(TREECKO),
    // 나무킹
    EC_POKEMON(SCEPTILE),
    // 나쁜
    EC_WORD_BADLY,
    // 나시
    EC_POKEMON_NATIONAL(EXEGGUTOR),
    // 나온다
    EC_WORD_APPEAR,
    // 나옹
    EC_POKEMON_NATIONAL(MEOWTH),
    // 나이스
    EC_WORD_NICE,
    // 나이트헤드
    EC_MOVE2(NIGHT_SHADE),
    // 나인테일
    EC_EMPTY_WORD, 2,
    EC_POKEMON(NINETALES),
    EC_POKEMON_NATIONAL(NINETALES),
    // 낙담
    EC_WORD_DOWNCAST,
    // 낙승
    EC_WORD_PUSHOVER,
    // 낚시
    EC_WORD_FISHING,
    // 난동부리기
    EC_MOVE(THRASH),
    // 날개치기
    EC_MOVE2(WING_ATTACK),
    // 날따름
    EC_MOVE(FOLLOW_ME),
    // 날려버리기
    EC_MOVE2(WHIRLWIND),
    // 날쌩마
    EC_POKEMON_NATIONAL(RAPIDASH),
    // 날씨부정
    EC_WORD_CLOUD_NINE,
    // 날카로운눈
    EC_WORD_KEEN_EYE,
    // 남자애
    EC_WORD_BOY,
    // 낮
    EC_WORD_DAYTIME,
    // 낮은
    EC_WORD_LOW,
    // 낮잠
    EC_WORD_NAP,
    // 내놔
    EC_WORD_GIMME,
    // 내루미
    EC_POKEMON_NATIONAL(LICKITUNG),
    // 내일
    EC_WORD_TOMORROW,
    // 냄새구별
    EC_MOVE(ODOR_SLEUTH),
    // 냄새꼬
    EC_EMPTY_WORD, 2,
    EC_POKEMON(GLOOM),
    EC_POKEMON_NATIONAL(GLOOM),
    // 냉동빔
    EC_MOVE2(ICE_BEAM),
    // 냉동펀치
    EC_MOVE2(ICE_PUNCH),
    // 너
    EC_WORD_HE,
    // 너는
    EC_WORD_HE_S,
    // 너덜너덜
    EC_WORD_SHREDDED,
    // 너로 정했다!
    EC_WORD_I_CHOOSE_YOU,
    // 너를
    EC_WORD_AREN_T,
    // 너무
    EC_WORD_EXCESS,
    // 너무 강하다
    EC_WORD_TOO_STRONG,
    // 너무 약하다
    EC_WORD_TOO_WEAK,
    // 너무 즐겁다
    EC_WORD_ENJOYS,
    // 너의
    EC_WORD_HIS,
    // 네가
    EC_WORD_PLACE,
    // 네게
    EC_WORD_DAUGHTER,
    // 네네
    EC_WORD_YEAH_YEAH,
    // 네오비트
    EC_POKEMON(ILLUMISE),
    // 네이티
    EC_EMPTY_WORD, 2,
    EC_POKEMON(NATU),
    EC_POKEMON_NATIONAL(NATU),
    // 네이티오
    EC_EMPTY_WORD, 2,
    EC_POKEMON(XATU),
    EC_POKEMON_NATIONAL(XATU),
    // 노
    EC_WORD_NO,
    // 노고치
    EC_POKEMON_NATIONAL(DUNSPARCE),
    // 노공룡
    EC_POKEMON(LOUDRED),
    // 노라키
    EC_POKEMON_NATIONAL(STANTLER),
    // 노래
    EC_WORD_SONG,
    // 노래
    EC_WORD_SONGS,
    // 노래하기
    EC_MOVE(SING),
    // 노린다
    EC_WORD_AIM,
    // 노말
    EC_WORD_NORMAL,
    // 녹기
    EC_MOVE2(ACID_ARMOR),
    // 논다
    EC_WORD_PLAYS,
    // 논외
    EC_WORD_QUESTION,
    // 놀래
    EC_WORD_PLAY,
    // 놀래키기
    EC_MOVE(ASTONISH),
    // 놀이
    EC_WORD_PLAYED,
    // 농담이야
    EC_WORD_JOKING,
    // 농담이지
    EC_WORD_BETTER,
    // 높이
    EC_WORD_HIGH,
    // 놓침
    EC_WORD_RELEASE,
    // 누가
    EC_WORD_WHO_WAS,
    // 누구
    EC_WORD_WHO,
    // 누구를
    EC_WORD_IT_S,
    // 누구에게
    EC_WORD_TO_WHOM,
    // 누구의
    EC_WORD_WHOSE,
    // 누군가
    EC_WORD_SOMEONE,
    // 누나
    EC_WORD_SISTER,
    // 누르기
    EC_MOVE2(BODY_SLAM),
    // 누오
    EC_POKEMON_NATIONAL(QUAGSIRE),
    // 눈꼬마
    EC_POKEMON(SNORUNT),
    // 눈보라
    EC_MOVE2(BLIZZARD),
    // 눈싸라기
    EC_MOVE(POWDER_SNOW),
    // 뉴스
    EC_WORD_NEWS,
    // 늪짱이
    EC_POKEMON(MARSHTOMP),
    // 니드런♀
    EC_POKEMON_NATIONAL(NIDORAN_F),
    // 니드런♂
    EC_POKEMON_NATIONAL(NIDORAN_M),
    // 니드리나
    EC_POKEMON_NATIONAL(NIDORINA),
    // 니드리노
    EC_POKEMON_NATIONAL(NIDORINO),
    // 니드퀸
    EC_POKEMON_NATIONAL(NIDOQUEEN),
    // 니드킹
    EC_POKEMON_NATIONAL(NIDOKING),
    // 니로우
    EC_POKEMON_NATIONAL(MURKROW),
    // 님
    EC_WORD_MR,
};

const u16 gEasyChatWordsByLetter_KoreanDigeut[] = {
    // 다
    EC_WORD_WAS,
    // 다꼬리
    EC_POKEMON_NATIONAL(FURRET),
    // 다른
    EC_WORD_DIFFERENT,
    // 다운
    EC_WORD_DID,
    // 다음
    EC_WORD_NEXT,
    // 다음에
    EC_WORD_LATER,
    // 다음 주
    EC_WORD_WEEK,
    // 다이렉트
    EC_WORD_DIRECT,
    // 다이빙
    EC_MOVE2(DIVE),
    // 다이어트
    EC_WORD_DIET,
    // 다크펫
    EC_POKEMON(BANETTE),
    // 다탱구
    EC_POKEMON(SHIFTRY),
    // 다행이다
    EC_WORD_GREAT,
    // 닥트리오
    EC_POKEMON_NATIONAL(DUGTRIO),
    // 단단지
    EC_POKEMON_NATIONAL(SHUCKLE),
    // 단단해지기
    EC_MOVE(HARDEN),
    // 단데기
    EC_POKEMON_NATIONAL(METAPOD),
    // 단련
    EC_WORD_TRAINS,
    // 달리기
    EC_WORD_RUNS,
    // 달린다
    EC_WORD_RUN,
    // 달빛
    EC_MOVE2(MOONLIGHT),
    // 달콤한향기
    EC_MOVE(SWEET_SCENT),
    // 당신
    EC_WORD_YOU,
    // 당신에게
    EC_WORD_SON,
    // 당신은
    EC_WORD_YOU_RE,
    // 당신을
    EC_WORD_YOU_VE,
    // 당신의
    EC_WORD_YOUR,
    // 당신이
    EC_WORD_YOURS,
    // 당연히
    EC_WORD_NATURALLY,
    // 당한다
    EC_WORD_APPEARS,
    // 당해서
    EC_WORD_BEAT,
    // 당했다
    EC_WORD_DEFEATED,
    // 대굴레오
    EC_POKEMON(SPHEAL),
    // 대단한
    EC_WORD_ELSE,
    // 대단히
    EC_WORD_AWFULLY,
    // 대신
    EC_WORD_INSTEAD,
    // 대전
    EC_WORD_VERSUS,
    // 대짱이
    EC_POKEMON(SWAMPERT),
    // 대타출동
    EC_MOVE2(SUBSTITUTE),
    // 대포무노
    EC_POKEMON_NATIONAL(OCTILLERY),
    // 대포무노포
    EC_MOVE(OCTAZOOKA),
    // 대폭발
    EC_MOVE2(EXPLOSION),
    // 대화
    EC_WORD_CONVERSATION,
    // 대회
    EC_WORD_TOURNAMENT,
    // 댄스
    EC_WORD_DANCE,
    // 더
    EC_WORD_THE,
    // 더블니들
    EC_MOVE2(TWINEEDLE),
    // 더 이상
    EC_WORD_TOO_LATE,
    // 던진다
    EC_WORD_THROW,
    // 덜컹덜컹
    EC_WORD_RICKETY,
    // 덩굴채찍
    EC_MOVE2(VINE_WHIP),
    // 덩쿠리
    EC_POKEMON_NATIONAL(TANGELA),
    // 데구리
    EC_POKEMON(GRAVELER),
    // 데구리
    EC_POKEMON_NATIONAL(GRAVELER),
    // 데기라스
    EC_POKEMON_NATIONAL(PUPITAR),
    // 데이트
    EC_WORD_DATE,
    // 델빌
    EC_POKEMON_NATIONAL(HOUNDOUR),
    // 델케티
    EC_POKEMON(DELCATTY),
    // 도
    EC_WORD_AROUND,
    // 도감
    EC_WORD_ENCYCLOPEDIA,
    // 도구
    EC_WORD_ITEM,
    // 도깨비불
    EC_MOVE(WILL_O_WISP),
    // 도나리
    EC_POKEMON_NATIONAL(VENOMOTH),
    // 도둑질
    EC_MOVE2(THIEF),
    // 도망
    EC_WORD_ESCAPE,
    // 도발
    EC_MOVE2(TAUNT),
    // 도우미
    EC_MOVE2(HELPING_HAND),
    // 도전
    EC_WORD_CHALLENGE,
    // 도주
    EC_WORD_RUN_AWAY,
    // 도 참
    EC_WORD_WILD,
    // 도토링
    EC_POKEMON(SEEDOT),
    // 독
    EC_WORD_POISON,
    // 독가루
    EC_MOVE2(POISON_POWDER),
    // 독가스
    EC_MOVE2(POISON_GAS),
    // 독가시
    EC_WORD_POISON_POINT,
    // 독침
    EC_MOVE2(POISON_STING),
    // 독침붕
    EC_POKEMON_NATIONAL(BEEDRILL),
    // 독케일
    EC_POKEMON(DUSTOX),
    // 독파리
    EC_POKEMON(TENTACRUEL),
    // 독파리
    EC_POKEMON_NATIONAL(TENTACRUEL),
    // 돈
    EC_WORD_MONEY,
    // 돌떨구기
    EC_MOVE(ROCK_THROW),
    // 돌려차기
    EC_MOVE2(ROLLING_KICK),
    // 돌머리
    EC_WORD_ROCK_HEAD,
    // 돌아갈래
    EC_WORD_GO_HOME,
    // 돌진
    EC_MOVE2(TAKE_DOWN),
    // 동료
    EC_WORD_ALLY,
    // 돼서
    EC_WORD_BECOMES,
    // 될래
    EC_WORD_LACKING,
    // 두근두근
    EC_WORD_EXCITING,
    // 두근두근
    EC_WORD_ANTICIPATION,
    // 두꺼운지방
    EC_WORD_THICK_FAT,
    // 두두
    EC_POKEMON(DODUO),
    // 두두
    EC_POKEMON_NATIONAL(DODUO),
    // 두번차기
    EC_MOVE2(DOUBLE_KICK),
    // 두코
    EC_POKEMON_NATIONAL(SKIPLOOM),
    // 두트리오
    EC_POKEMON(DODRIO),
    // 두트리오
    EC_POKEMON_NATIONAL(DODRIO),
    // 둔감
    EC_WORD_OBLIVIOUS,
    // 둔타
    EC_POKEMON(NUMEL),
    // 뒤에
    EC_WORD_BACK,
    // 드디어
    EC_WORD_FINALLY,
    // 드래곤
    EC_WORD_DRAGON,
    // 드래곤클로
    EC_MOVE2(DRAGON_CLAW),
    // 드립니다
    EC_WORD_WILL,
    // 듣는다
    EC_WORD_HEARING,
    // 들떠서
    EC_WORD_GIDDY,
    // 들어서
    EC_WORD_LISTENS,
    // 들을래
    EC_WORD_HEAR,
    // 듯하다
    EC_WORD_SEEMS,
    // 등장
    EC_WORD_I_VE_ARRIVED,
    // 디그다
    EC_POKEMON_NATIONAL(DIGLETT),
    // 디자인
    EC_WORD_DESIGN,
    // 디지털
    EC_WORD_DIGITAL,
    // 딜리버드
    EC_POKEMON_NATIONAL(DELIBIRD),
};

const u16 gEasyChatWordsByLetter_KoreanSsangDigeut[] = {
    // 따라가때리기
    EC_MOVE(PURSUIT),
    // 따라하기
    EC_MOVE(MIRROR_MOVE),
    // 딱구리
    EC_POKEMON(GOLEM),
    // 딱구리
    EC_POKEMON_NATIONAL(GOLEM),
    // 딱딱
    EC_WORD_ROCK_SOLID,
    // 딱 좋아
    EC_WORD_WINNER,
    // 딱충이
    EC_POKEMON_NATIONAL(KAKUNA),
    // 딱히
    EC_WORD_NOT_VERY,
    // 땅
    EC_WORD_GROUND,
    // 땅가르기
    EC_MOVE(FISSURE),
    // 때
    EC_WORD_AGE,
    // 땡큐
    EC_WORD_THANK_YOU,
    // 떨어진
    EC_WORD_AWAY,
    // 또가스
    EC_POKEMON(KOFFING),
    // 또가스
    EC_POKEMON_NATIONAL(KOFFING),
    // 또는
    EC_WORD_OR,
    // 또도가스
    EC_POKEMON(WEEZING),
    // 또도가스
    EC_POKEMON_NATIONAL(WEEZING),
    // 또 봐
    EC_WORD_SMELL,
    // 뚜벅쵸
    EC_POKEMON(ODDISH),
    // 뚜벅쵸
    EC_POKEMON_NATIONAL(ODDISH),
    // 뛰어오르기
    EC_MOVE2(BOUNCE),
    // 뜨거운
    EC_WORD_HOT,
    // 띠-잉
    EC_WORD_SHOCKED,
};

const u16 gEasyChatWordsByLetter_KoreanRieul[] = {
    // 라고
    EC_WORD_THAT_S,
    // 라고 해서
    EC_WORD_IT,
    // 라구요
    EC_WORD_SO,
    // 라기보단
    EC_WORD_JUST,
    // 라디오
    EC_WORD_RADIO,
    // 라이벌
    EC_WORD_RIVAL,
    // 라이츄
    EC_EMPTY_WORD, 2,
    EC_POKEMON(RAICHU),
    EC_POKEMON_NATIONAL(RAICHU),
    // 라이코
    EC_POKEMON_NATIONAL(RAIKOU),
    // 라이프
    EC_WORD_LIFE,
    // 라티아스
    EC_POKEMON(LATIAS),
    // 라티오스
    EC_POKEMON(LATIOS),
    // 라프라스
    EC_POKEMON_NATIONAL(LAPRAS),
    // 라플레시아
    EC_EMPTY_WORD, 2,
    EC_POKEMON(VILEPLUME),
    EC_POKEMON_NATIONAL(VILEPLUME),
    // 란 건 없다
    EC_WORD_WEREN_T,
    // 랄라라
    EC_WORD_LALALA,
    // 랄토스
    EC_POKEMON(RALTS),
    // 랍니다
    EC_WORD_MAYBE,
    // 랜턴
    EC_EMPTY_WORD, 2,
    EC_POKEMON(LANTURN),
    EC_POKEMON_NATIONAL(LANTURN),
    // 랭크
    EC_WORD_RANK,
    // 러브러브
    EC_WORD_LOVEY_DOVEY,
    // 러스터퍼지
    EC_MOVE2(LUSTER_PURGE),
    // 럭키
    EC_POKEMON_NATIONAL(CHANSEY),
    // 레드
    EC_WORD_RED,
    // 레디바
    EC_POKEMON_NATIONAL(LEDYBA),
    // 레디안
    EC_POKEMON_NATIONAL(LEDIAN),
    // 레벨
    EC_WORD_LEVEL,
    // 레어코일
    EC_EMPTY_WORD, 2,
    EC_POKEMON(MAGNETON),
    EC_POKEMON_NATIONAL(MAGNETON),
    // 레지락
    EC_POKEMON(REGIROCK),
    // 레지스틸
    EC_POKEMON(REGISTEEL),
    // 레지아이스
    EC_POKEMON(REGICE),
    // 레쿠쟈
    EC_POKEMON(RAYQUAZA),
    // 레트라
    EC_POKEMON_NATIONAL(RATICATE),
    // 렌탈
    EC_WORD_RENTAL,
    // 로망
    EC_WORD_ROMANTIC,
    // 로젤리아
    EC_POKEMON(ROSELIA),
    // 로켓박치기
    EC_MOVE2(SKULL_BASH),
    // 로토스
    EC_POKEMON(LOMBRE),
    // 로파파
    EC_POKEMON(LUDICOLO),
    // 록블라스트
    EC_MOVE2(ROCK_BLAST),
    // 록온
    EC_MOVE2(LOCK_ON),
    // 롱스톤
    EC_POKEMON_NATIONAL(ONIX),
    // 루기아
    EC_POKEMON_NATIONAL(LUGIA),
    // 루나톤
    EC_POKEMON(LUNATONE),
    // 루리리
    EC_POKEMON(AZURILL),
    // 루브도
    EC_POKEMON_NATIONAL(SMEARGLE),
    // 루비
    EC_WORD_RUBY,
    // 루주라
    EC_POKEMON_NATIONAL(JYNX),
    // 룰
    EC_WORD_RULE,
    // 룰루랄라
    EC_WORD_HAPPILY,
    // 룸
    EC_WORD_ROOM,
    // 리더
    EC_WORD_LEADER,
    // 리벤지
    EC_MOVE2(REVENGE),
    // 리본
    EC_WORD_RIBBON,
    // 리사이클
    EC_MOVE2(RECYCLE),
    // 리아코
    EC_POKEMON_NATIONAL(TOTODILE),
    // 리자드
    EC_POKEMON_NATIONAL(CHARMELEON),
    // 리자몽
    EC_POKEMON_NATIONAL(CHARIZARD),
    // 리프
    EC_WORD_LEAF,
    // 리프레시
    EC_MOVE2(REFRESH),
    // 리프블레이드
    EC_MOVE2(LEAF_BLADE),
    // 리플렉터
    EC_MOVE2(REFLECT),
    // 릴리요
    EC_POKEMON(CRADILY),
    // 릴링
    EC_POKEMON(LILEEP),
    // 링곰
    EC_POKEMON_NATIONAL(URSARING),
};

const u16 gEasyChatWordsByLetter_KoreanMieum[] = {
    // 마구
    EC_WORD_OVERDO,
    // 마구찌르기
    EC_MOVE2(FURY_ATTACK),
    // 마구할퀴기
    EC_MOVE2(FURY_SWIPES),
    // 마그마
    EC_POKEMON_NATIONAL(MAGMAR),
    // 마그마그
    EC_EMPTY_WORD, 2,
    EC_POKEMON(SLUGMA),
    EC_POKEMON_NATIONAL(SLUGMA),
    // 마그마의무장
    EC_WORD_MAGMA_ARMOR,
    // 마그비
    EC_POKEMON_NATIONAL(MAGBY),
    // 마그카르고
    EC_EMPTY_WORD, 2,
    EC_POKEMON(MAGCARGO),
    EC_POKEMON_NATIONAL(MAGCARGO),
    // 마그케인
    EC_POKEMON_NATIONAL(QUILAVA),
    // 마기라스
    EC_POKEMON_NATIONAL(TYRANITAR),
    // 마릴
    EC_EMPTY_WORD, 2,
    EC_POKEMON(MARILL),
    EC_POKEMON_NATIONAL(MARILL),
    // 마릴리
    EC_EMPTY_WORD, 2,
    EC_POKEMON(AZUMARILL),
    EC_POKEMON_NATIONAL(AZUMARILL),
    // 마스터
    EC_WORD_MASTER,
    // 마신다
    EC_WORD_DRINKS,
    // 마실래
    EC_WORD_DRINK,
    // 마음의눈
    EC_MOVE(MIND_READER),
    // 마이너스
    EC_WORD_MINUS,
    // 마이농
    EC_POKEMON(MINUN),
    // 마이페이스
    EC_WORD_OWN_TEMPO,
    // 마임맨
    EC_POKEMON_NATIONAL(MR_MIME),
    // 마자
    EC_POKEMON(WYNAUT),
    // 마자용
    EC_EMPTY_WORD, 2,
    EC_POKEMON(WOBBUFFET),
    EC_POKEMON_NATIONAL(WOBBUFFET),
    // 마치
    EC_WORD_AS_IF,
    // 마침내
    EC_WORD_FINAL,
    // 마크탕
    EC_POKEMON(MAKUHITA),
    // 마하펀치
    EC_MOVE2(MACH_PUNCH),
    // 막치기
    EC_MOVE2(POUND),
    // 만난다
    EC_WORD_MEETS,
    // 만날래
    EC_WORD_MEET,
    // 만든다
    EC_WORD_MAKE,
    // 만들기
    EC_WORD_MAKING,
    // 만약
    EC_WORD_IF,
    // 만족
    EC_WORD_SATISFIED,
    // 만큼
    EC_WORD_AS_MUCH_AS,
    // 만타인
    EC_POKEMON_NATIONAL(MANTINE),
    // 만화
    EC_WORD_COMICS,
    // 만화영화
    EC_WORD_ANIME,
    // 많습니다
    EC_WORD_COULD,
    // 말
    EC_WORD_WORD,
    // 말괄량이
    EC_WORD_CANCEL,
    // 말도 안 돼
    EC_WORD_KIDDING,
    // 말하고
    EC_WORD_SAID,
    // 말한다
    EC_WORD_SAYS,
    // 맛있는
    EC_WORD_TASTY,
    // 망각술
    EC_MOVE2(AMNESIA),
    // 망나뇽
    EC_POKEMON_NATIONAL(DRAGONITE),
    // 망키
    EC_POKEMON_NATIONAL(MANKEY),
    // 맞죠?
    EC_WORD_AN,
    // 매그니튜드
    EC_MOVE2(MAGNITUDE),
    // 매우
    EC_WORD_VERY,
    // 매우 굉장한
    EC_WORD_OVERWHELMING,
    // 매일
    EC_WORD_DAILY,
    // 매지컬리프
    EC_MOVE2(MAGICAL_LEAF),
    // 매직코트
    EC_MOVE2(MAGIC_COAT),
    // 맹독
    EC_MOVE2(TOXIC),
    // 맹독엄니
    EC_MOVE2(POISON_FANG),
    // 맹렬
    EC_WORD_OLD,
    // 맹화
    EC_WORD_BLAZE,
    // 머드샷
    EC_MOVE2(MUD_SHOT),
    // 머신
    EC_WORD_MACHINE,
    // 머지않아
    EC_WORD_NEARLY,
    // 먹는다
    EC_WORD_EATS,
    // 먹을래
    EC_WORD_EAT,
    // 먼
    EC_WORD_FAR,
    // 멀리짖기
    EC_MOVE2(HOWL),
    // 멋져
    EC_WORD_MEGA,
    // 멋진
    EC_WORD_FABULOUS,
    // 메가니움
    EC_POKEMON_NATIONAL(MEGANIUM),
    // 메가드레인
    EC_MOVE2(MEGA_DRAIN),
    // 메가톤킥
    EC_MOVE2(MEGA_KICK),
    // 메가톤펀치
    EC_MOVE2(MEGA_PUNCH),
    // 메가혼
    EC_MOVE2(MEGAHORN),
    // 메깅
    EC_POKEMON(WHISCASH),
    // 메꾸리
    EC_POKEMON_NATIONAL(PILOSWINE),
    // 메리프
    EC_POKEMON_NATIONAL(MAREEP),
    // 메시지
    EC_WORD_MESSAGE,
    // 메일
    EC_WORD_MAIL,
    // 메타그로스
    EC_POKEMON(METAGROSS),
    // 메타몽
    EC_POKEMON_NATIONAL(DITTO),
    // 메탈클로
    EC_MOVE2(METAL_CLAW),
    // 메탕
    EC_POKEMON(BELDUM),
    // 메탕구
    EC_POKEMON(METANG),
    // 면역
    EC_WORD_IMMUNITY,
    // 멸망의노래
    EC_MOVE2(PERISH_SONG),
    // 명상
    EC_MOVE2(CALM_MIND),
    // 모다피
    EC_POKEMON_NATIONAL(BELLSPROUT),
    // 모두
    EC_WORD_WE,
    // 모두가
    EC_WORD_BEEN,
    // 모두는
    EC_WORD_WE_RE,
    // 모두에게
    EC_WORD_TO_US,
    // 모두의
    EC_WORD_OUR,
    // 모드
    EC_WORD_MODE,
    // 모든
    EC_WORD_EVERY,
    // 모래날림
    EC_WORD_SAND_STREAM,
    // 모래두지
    EC_EMPTY_WORD, 2,
    EC_POKEMON(SANDSHREW),
    EC_POKEMON_NATIONAL(SANDSHREW),
    // 모래바람
    EC_MOVE(SANDSTORM),
    // 모래뿌리기
    EC_MOVE(SAND_ATTACK),
    // 모래숨기
    EC_WORD_SAND_VEIL,
    // 모래지옥
    EC_MOVE(SAND_TOMB),
    // 모른다
    EC_WORD_IGNORANT,
    // 모양
    EC_WORD_PATTERN,
    // 모으기
    EC_WORD_COLLECT,
    // 모험
    EC_WORD_ADVENTURE,
    // 목숨
    EC_WORD_DESTINY,
    // 목요일
    EC_WORD_THURSDAY,
    // 목욕
    EC_WORD_BATH,
    // 목화포자
    EC_MOVE2(COTTON_SPORE),
    // 몸통박치기
    EC_MOVE2(TACKLE),
    // 못 기다려
    EC_WORD_WAIT,
    // 못 당한다
    EC_WORD_NO_MATCH,
    // 무드
    EC_WORD_MOOD,
    // 무릎차기
    EC_MOVE2(HIGH_JUMP_KICK),
    // 무서운
    EC_WORD_SCARY,
    // 무선
    EC_WORD_WIRELESS,
    // 무엇
    EC_WORD_WHAT,
    // 무우마
    EC_POKEMON_NATIONAL(MISDREAVUS),
    // 무장조
    EC_EMPTY_WORD, 2,
    EC_POKEMON(SKARMORY),
    EC_POKEMON_NATIONAL(SKARMORY),
    // 무적
    EC_WORD_INVINCIBLE,
    // 물
    EC_WORD_WATER,
    // 물기
    EC_MOVE(BITE),
    // 물놀이
    EC_MOVE2(WATER_SPORT),
    // 물대포
    EC_MOVE2(WATER_GUN),
    // 물의파동
    EC_MOVE2(WATER_PULSE),
    // 물짱이
    EC_POKEMON(MUDKIP),
    // 뭐?
    EC_WORD_EH_QUES,
    // 뭐든지
    EC_WORD_READY,
    // 뭐야
    EC_WORD_WHAT_S_UP_QUES,
    // 뮤
    EC_POKEMON_NATIONAL(MEW),
    // 뮤츠
    EC_POKEMON_NATIONAL(MEWTWO),
    // 미꾸리
    EC_POKEMON(BARBOACH),
    // 미끈미끈
    EC_WORD_SLIMY,
    // 미뇽
    EC_POKEMON_NATIONAL(DRATINI),
    // 미라몽
    EC_POKEMON(DUSCLOPS),
    // 미래예지
    EC_MOVE2(FUTURE_SIGHT),
    // 미러코트
    EC_MOVE2(MIRROR_COAT),
    // 미스트볼
    EC_MOVE2(MIST_BALL),
    // 미식가
    EC_WORD_GOURMET,
    // 미안
    EC_WORD_SORRY,
    // 미안해
    EC_WORD_FORGIVE,
    // 미안해요
    EC_WORD_APOLOGIZE,
    // 미지근한
    EC_WORD_LUKEWARM,
    // 믿고서
    EC_WORD_BELIEVE,
    // 믿을 수 없어
    EC_WORD_INCREDIBLE,
    // 밀로틱
    EC_POKEMON(MILOTIC),
    // 밀탱크
    EC_POKEMON_NATIONAL(MILTANK),
    // 밑에
    EC_WORD_BELOW,
};

const u16 gEasyChatWordsByLetter_KoreanBieup[] = {
    // 바꾼다
    EC_WORD_CHANGE,
    // 바늘미사일
    EC_MOVE2(PIN_MISSILE),
    // 바늘팔
    EC_MOVE2(NEEDLE_ARM),
    // 바다회오리
    EC_MOVE(WHIRLPOOL),
    // 바둥바둥
    EC_MOVE(FLAIL),
    // 바람일으키기
    EC_MOVE(GUST),
    // 바로
    EC_WORD_HOWEVER,
    // 바보야
    EC_WORD_NITWIT,
    // 바쁜
    EC_WORD_BUSY,
    // 바위
    EC_WORD_ROCK,
    // 바위깨기
    EC_MOVE(ROCK_SMASH),
    // 바이바이
    EC_WORD_BYE_BYE,
    // 바이바이
    EC_WORD_SMELL_YA,
    // 박치기
    EC_MOVE(HEADBUTT),
    // 밖에
    EC_WORD_OUTSIDE,
    // 반가워요
    EC_WORD_MEET_YOU,
    // 반대편
    EC_WORD_OPPOSITE,
    // 반드시
    EC_WORD_MUST_BE,
    // 반딧불
    EC_MOVE2(TAIL_GLOW),
    // 받아던지기
    EC_MOVE(VITAL_THROW),
    // 받아라
    EC_WORD_TAKE_THAT,
    // 받았다
    EC_WORD_RECEIVED,
    // 발광
    EC_WORD_ILLUMINATE,
    // 발바로
    EC_POKEMON(VIGOROTH),
    // 발버둥
    EC_MOVE2(STRUGGLE),
    // 발챙이
    EC_POKEMON_NATIONAL(POLIWAG),
    // 밤
    EC_WORD_NIGHT,
    // 밤선인
    EC_POKEMON(CACTURNE),
    // 밤중
    EC_WORD_NIGHTTIME,
    // 방어
    EC_MOVE2(PROTECT),
    // 방음
    EC_WORD_SOUNDPROOF,
    // 방학
    EC_WORD_VACATION,
    // 배루키
    EC_POKEMON_NATIONAL(TYROGUE),
    // 배리어
    EC_MOVE2(BARRIER),
    // 배북
    EC_MOVE2(BELLY_DRUM),
    // 배움
    EC_WORD_LEARN,
    // 배턴터치
    EC_MOVE2(BATON_PASS),
    // 배틀
    EC_WORD_BATTLE,
    // 배틀룸
    EC_WORD_BATTLE_ROOM,
    // 배틀리더
    EC_WORD_LEADERS,
    // 배틀타워
    EC_WORD_BATTLE_TOWER,
    // 백화점
    EC_WORD_DEPT_STORE,
    // 뱀눈초리
    EC_MOVE2(GLARE),
    // 뱅글뱅글
    EC_WORD_SPIRALING,
    // 버릇
    EC_WORD_HABIT,
    // 버섯꼬
    EC_POKEMON(SHROOMISH),
    // 버섯모
    EC_POKEMON(BRELOOM),
    // 버섯포자
    EC_MOVE(SPORE),
    // 버전
    EC_WORD_VERSION,
    // 버터플
    EC_POKEMON_NATIONAL(BUTTERFREE),
    // 버티기
    EC_MOVE(ENDURE),
    // 번
    EC_WORD_TIMES,
    // 번개
    EC_MOVE(THUNDER),
    // 번개펀치
    EC_MOVE(THUNDER_PUNCH),
    // 번치코
    EC_POKEMON(BLAZIKEN),
    // 벌레
    EC_WORD_BUG,
    // 벌레의알림
    EC_WORD_SWARM,
    // 벌크업
    EC_MOVE2(BULK_UP),
    // 베스트
    EC_WORD_BEST,
    // 베어가르기
    EC_MOVE(SLASH),
    // 베이리프
    EC_POKEMON_NATIONAL(BAYLEEF),
    // 베이베
    EC_WORD_BABY,
    // 변색
    EC_WORD_COLOR_CHANGE,
    // 변신
    EC_MOVE2(TRANSFORM),
    // 별가사리
    EC_EMPTY_WORD, 2,
    EC_POKEMON(STARYU),
    EC_POKEMON_NATIONAL(STARYU),
    // 보다
    EC_WORD_THAN,
    // 보드
    EC_WORD_BOARD,
    // 보만다
    EC_POKEMON(SALAMENCE),
    // 보물
    EC_WORD_TREASURE,
    // 보송송
    EC_POKEMON_NATIONAL(FLAAFFY),
    // 보스로라
    EC_POKEMON(AGGRON),
    // 보슬보슬
    EC_WORD_SILKY,
    // 보여서
    EC_WORD_SHOW,
    // 보호
    EC_WORD_GUARD,
    // 보호색
    EC_MOVE2(CAMOUFLAGE),
    // 복안
    EC_WORD_COMPOUNDEYES,
    // 본다
    EC_WORD_SEES,
    // 본러시
    EC_MOVE2(BONE_RUSH),
    // 볼
    EC_WORD_BALL,
    // 볼비트
    EC_POKEMON(VOLBEAT),
    // 볼트태클
    EC_MOVE2(VOLT_TACKLE),
    // 봄
    EC_WORD_SPRING,
    // 봉인
    EC_MOVE2(IMPRISON),
    // 봉제인형
    EC_WORD_PLUSH_DOLL,
    // 봐서
    EC_WORD_LOOKS,
    // 부끄부끄
    EC_WORD_HEHE,
    // 부들부들
    EC_WORD_SHAKY,
    // 부딪힘
    EC_WORD_SMACK,
    // 부스터
    EC_POKEMON_NATIONAL(FLAREON),
    // 부우부
    EC_POKEMON_NATIONAL(HOOTHOOT),
    // 부유
    EC_WORD_LEVITATE,
    // 부족
    EC_WORD_LACKS,
    // 부추기기
    EC_MOVE(FLATTER),
    // 부탁
    EC_WORD_PLEASE,
    // 부활
    EC_WORD_RESUSCITATE,
    // 분노
    EC_MOVE(RAGE),
    // 분노의앞니
    EC_MOVE(SUPER_FANG),
    // 분한
    EC_WORD_ANGRY,
    // 분함
    EC_WORD_ANGER,
    // 분홍장이
    EC_POKEMON(GOREBYSS),
    // 분화
    EC_MOVE2(ERUPTION),
    // 불가능한
    EC_WORD_IMPOSSIBLE,
    // 불가사의부적
    EC_WORD_WONDER_GUARD,
    // 불가피한
    EC_WORD_UNAVOIDABLE,
    // 불꽃
    EC_WORD_FIRE,
    // 불꽃몸
    EC_WORD_FLAME_BODY,
    // 불꽃세례
    EC_MOVE2(EMBER),
    // 불꽃펀치
    EC_MOVE2(FIRE_PUNCH),
    // 불대문자
    EC_MOVE2(FIRE_BLAST),
    // 불면
    EC_WORD_INSOMNIA,
    // 불새
    EC_MOVE(SKY_ATTACK),
    // 불안
    EC_WORD_FEAR,
    // 붐볼
    EC_EMPTY_WORD, 2,
    EC_POKEMON(ELECTRODE),
    EC_POKEMON_NATIONAL(ELECTRODE),
    // 뷰티플라이
    EC_POKEMON(BEAUTIFLY),
    // 브레이크클로
    EC_MOVE2(CRUSH_CLAW),
    // 브케인
    EC_POKEMON_NATIONAL(CYNDAQUIL),
    // 블라스트번
    EC_MOVE2(BLAST_BURN),
    // 블래키
    EC_POKEMON_NATIONAL(UMBREON),
    // 블레이범
    EC_POKEMON_NATIONAL(TYPHLOSION),
    // 블레이즈킥
    EC_MOVE2(BLAZE_KICK),
    // 블렌드
    EC_WORD_BLEND,
    // 블록
    EC_MOVE2(BLOCK),
    // 블루
    EC_POKEMON_NATIONAL(SNUBBULL),
    // 비구술
    EC_POKEMON(SURSKIT),
    // 비나방
    EC_POKEMON(MASQUERAIN),
    // 비밀
    EC_WORD_SECRET,
    // 비밀기지
    EC_WORD_SECRET_BASE,
    // 비밀의힘
    EC_MOVE2(SECRET_POWER),
    // 비바라기
    EC_MOVE(RAIN_DANCE),
    // 비브라바
    EC_POKEMON(VIBRAVA),
    // 비싼
    EC_WORD_EXPENSIVE,
    // 비전
    EC_WORD_HIDDEN,
    // 비축하기
    EC_MOVE2(STOCKPILE),
    // 비행
    EC_WORD_FLYING,
    // 빈티나
    EC_POKEMON(FEEBAS),
    // 빙글빙글
    EC_WORD_TWIRLING,
    // 빛난다
    EC_WORD_SHINE,
    // 빛의장막
    EC_MOVE2(LIGHT_SCREEN),
};

const u16 gEasyChatWordsByLetter_KoreanSsangBieup[] = {
    // 빠끔빠끔
    EC_WORD_VORACIOUS,
    // 빠른
    EC_WORD_FAST,
    // 빠져서
    EC_WORD_SINK,
    // 빵 터짐
    EC_WORD_BANG,
    // 뻗었다
    EC_WORD_KNOCKOUT,
    // 뼈다귀부메랑
    EC_MOVE2(BONEMERANG),
    // 뼈다귀치기
    EC_MOVE2(BONE_CLUB),
    // 뽀뽀라
    EC_POKEMON_NATIONAL(SMOOCHUM),
    // 뽐내기
    EC_MOVE(SWAGGER),
    // 뿌리박기
    EC_MOVE2(INGRAIN),
    // 뿐
    EC_WORD_ONLY,
    // 뿔드릴
    EC_MOVE2(HORN_DRILL),
    // 뿔찌르기
    EC_MOVE2(HORN_ATTACK),
    // 뿔충이
    EC_POKEMON_NATIONAL(WEEDLE),
    // 뿔카노
    EC_EMPTY_WORD, 2,
    EC_POKEMON(RHYHORN),
    EC_POKEMON_NATIONAL(RHYHORN),
    // 쁘사이저
    EC_EMPTY_WORD, 2,
    EC_POKEMON(PINSIR),
    EC_POKEMON_NATIONAL(PINSIR),
    // 삐
    EC_POKEMON_NATIONAL(CLEFFA),
    // 삐삐
    EC_POKEMON_NATIONAL(CLEFAIRY),
};

const u16 gEasyChatWordsByLetter_KoreanSiot[] = {
    // 사건
    EC_WORD_EVENT,
    // 사람
    EC_WORD_PERSON,
    // 사랑동이
    EC_POKEMON(LUVDISC),
    // 사슬묶기
    EC_MOVE(DISABLE),
    // 사이코부스트
    EC_MOVE(PSYCHO_BOOST),
    // 사이코웨이브
    EC_MOVE(PSYWAVE),
    // 사이코키네시스
    EC_MOVE(PSYCHIC),
    // 사파이어
    EC_WORD_SAPPHIRE,
    // 사회자
    EC_WORD_MC,
    // 산책
    EC_WORD_WALK,
    // 상당히
    EC_WORD_QUITE,
    // 상대
    EC_WORD_OPPONENT,
    // 상대방
    EC_WORD_FOE,
    // 상성
    EC_WORD_MATCH_UP,
    // 새것
    EC_WORD_NEW,
    // 색
    EC_WORD_COLOR,
    // 색이 다른
    EC_WORD_ALT_COLOR,
    // 생각든다
    EC_WORD_THINK,
    // 생각한다
    EC_WORD_THINKS,
    // 생김새
    EC_WORD_LOOK,
    // 생활
    EC_WORD_LIVING,
    // 샤미드
    EC_POKEMON_NATIONAL(VAPOREON),
    // 샤크니아
    EC_POKEMON(SHARPEDO),
    // 샤프니아
    EC_POKEMON(CARVANHA),
    // 섀도볼
    EC_MOVE(SHADOW_BALL),
    // 섀도펀치
    EC_MOVE(SHADOW_PUNCH),
    // 서두른다
    EC_WORD_GIVES,
    // 서둘러서
    EC_WORD_HURRIED,
    // 서비스
    EC_WORD_SERVICE,
    // 서투른
    EC_WORD_LOUSY,
    // 선생님
    EC_WORD_TEACHER,
    // 선인왕
    EC_POKEMON(CACNEA),
    // 선택
    EC_WORD_CHOICE,
    // 성격
    EC_WORD_NATURE,
    // 성스러운불꽃
    EC_MOVE(SACRED_FIRE),
    // 성원숭
    EC_POKEMON_NATIONAL(PRIMEAPE),
    // 성장
    EC_MOVE(GROWTH),
    // 세레비
    EC_POKEMON_NATIONAL(CELEBI),
    // 세련
    EC_WORD_MODERN,
    // 세비퍼
    EC_POKEMON(SEVIPER),
    // 센스
    EC_WORD_SENSE,
    // 센터
    EC_WORD_CENTER,
    // 셀러
    EC_POKEMON_NATIONAL(SHELLDER),
    // 소곤룡
    EC_POKEMON(WHISMUR),
    // 소꿉놀이
    EC_WORD_CHILD_S_PLAY,
    // 소닉붐
    EC_MOVE(SONIC_BOOM),
    // 소란
    EC_WORD_CAUSE,
    // 소란피기
    EC_MOVE(UPROAR),
    // 소중히
    EC_WORD_CARES,
    // 소프트
    EC_WORD_SOFTWARE,
    // 속여때리기
    EC_MOVE2(FEINT_ATTACK),
    // 속이기
    EC_MOVE2(FAKE_OUT),
    // 손가락흔들기
    EC_MOVE2(METRONOME),
    // 손바닥치기
    EC_MOVE2(ARM_THRUST),
    // 솔라빔
    EC_MOVE(SOLAR_BEAM),
    // 솔록
    EC_POKEMON(SOLROCK),
    // 솜솜코
    EC_POKEMON_NATIONAL(JUMPLUFF),
    // 쇼핑
    EC_WORD_SHOPPING,
    // 수다쟁이
    EC_WORD_CHAT,
    // 수면가루
    EC_MOVE2(SLEEP_POWDER),
    // 수업
    EC_WORD_CLASS,
    // 수요일
    EC_WORD_WEDNESDAY,
    // 수의베일
    EC_WORD_WATER_VEIL,
    // 수행
    EC_WORD_TRAIN,
    // 순간이동
    EC_MOVE2(TELEPORT),
    // 순수한힘
    EC_WORD_PURE_POWER,
    // 숟가락휘기
    EC_MOVE(KINESIS),
    // 쉘곤
    EC_POKEMON(SHELGON),
    // 쉬엄쉬엄
    EC_WORD_GO_EASY,
    // 쉬움
    EC_WORD_SIMPLE,
    // 쉽다
    EC_WORD_EASY,
    // 슈륙챙이
    EC_POKEMON_NATIONAL(POLIWHIRL),
    // 슈퍼
    EC_WORD_SUPER,
    // 스라크
    EC_POKEMON_NATIONAL(SCYTHER),
    // 스모그
    EC_MOVE(SMOG),
    // 스왈로
    EC_POKEMON(SWELLOW),
    // 스이쿤
    EC_POKEMON_NATIONAL(SUICUNE),
    // 스카이어퍼
    EC_MOVE(SKY_UPPERCUT),
    // 스케치
    EC_MOVE(SKETCH),
    // 스킬스왑
    EC_MOVE(SKILL_SWAP),
    // 스타트
    EC_WORD_START,
    // 스테이지
    EC_WORD_STAGE,
    // 스토리
    EC_WORD_STORY,
    // 스톤샤워
    EC_MOVE(ROCK_SLIDE),
    // 스톱
    EC_WORD_STOP,
    // 스파크
    EC_MOVE(SPARK),
    // 스포츠
    EC_WORD_SPORTS,
    // 스피드스타
    EC_MOVE(SWIFT),
    // 슬기로운
    EC_WORD_SMART,
    // 슬기로움
    EC_WORD_SMARTNESS,
    // 슬리퍼
    EC_POKEMON_NATIONAL(HYPNO),
    // 슬리프
    EC_POKEMON_NATIONAL(DROWZEE),
    // 슬픈
    EC_WORD_SAD,
    // 습기
    EC_WORD_DAMP,
    // 승
    EC_WORD_WINS,
    // 승리
    EC_WORD_VICTORY,
    // 승부
    EC_WORD_MATCH,
    // 시간
    EC_WORD_TIME,
    // 시그널빔
    EC_MOVE(SIGNAL_BEAM),
    // 시끄러운
    EC_WORD_NOISY,
    // 시드라
    EC_POKEMON(SEADRA),
    // 시드라
    EC_POKEMON_NATIONAL(SEADRA),
    // 시라소몬
    EC_POKEMON_NATIONAL(HITMONLEE),
    // 시라칸
    EC_POKEMON(RELICANTH),
    // 시스템
    EC_WORD_SYSTEM,
    // 시원한
    EC_WORD_REFRESHING,
    // 시작
    EC_WORD_BEGINNING,
    // 시험
    EC_WORD_TEST,
    // 식스테일
    EC_POKEMON(VULPIX),
    // 식스테일
    EC_POKEMON_NATIONAL(VULPIX),
    // 신경쓰인다
    EC_WORD_WANTS,
    // 신경 안 써
    EC_WORD_HEARS,
    // 신나게
    EC_WORD_UPBEAT,
    // 신뇽
    EC_POKEMON_NATIONAL(DRAGONAIR),
    // 신비의부적
    EC_MOVE(SAFEGUARD),
    // 신속
    EC_MOVE(EXTREME_SPEED),
    // 신통력
    EC_MOVE(EXTRASENSORY),
    // 실례한다
    EC_WORD_PARDON,
    // 실례할게
    EC_WORD_EXCUSE,
    // 실망
    EC_WORD_DISAPPOINTED,
    // 실망했다
    EC_WORD_DISAPPOINTS,
    // 실버
    EC_WORD_SILVER,
    // 실뿜기
    EC_MOVE(STRING_SHOT),
    // 실수로
    EC_WORD_MISTAKE,
    // 실쿤
    EC_POKEMON(SILCOON),
    // 싫어
    EC_WORD_BEING,
    // 싫어
    EC_WORD_NEGATIVE,
    // 싫어한다
    EC_WORD_DISLIKE,
    // 싫은소리
    EC_MOVE(SCREECH),
    // 심록
    EC_WORD_OVERGROW,
    // 심사위원
    EC_WORD_JUDGE,
    // 심쿵
    EC_WORD_HEART,
    // 심판
    EC_WORD_SMITE,
    // 심한
    EC_WORD_AWFUL,
    // 싱크로
    EC_WORD_SYNCHRONIZE,
};

const u16 gEasyChatWordsByLetter_KoreanSsangSiot[] = {
    // 싸라기눈
    EC_MOVE(HAIL),
    // 싸움
    EC_WORD_FIGHTS,
    // 써서
    EC_WORD_USING,
    // 썬더
    EC_POKEMON_NATIONAL(ZAPDOS),
    // 썬더라이
    EC_POKEMON(ELECTRIKE),
    // 썬더볼트
    EC_POKEMON(MANECTRIC),
    // 쏘드라
    EC_EMPTY_WORD, 2,
    EC_POKEMON(HORSEA),
    EC_POKEMON_NATIONAL(HORSEA),
    // 쏘콘
    EC_POKEMON_NATIONAL(FORRETRESS),
    // 쓰기
    EC_WORD_USES,
    // 쓰담쓰담
    EC_WORD_SMOOTH,
    // 쓱쓱
    EC_WORD_SWIFT_SWIM,
    // 쓴다
    EC_WORD_USE,
    // 씨
    EC_WORD_MRS,
    // 씨기관총
    EC_MOVE2(BULLET_SEED),
    // 씨레오
    EC_POKEMON(SEALEO),
    // 씨뿌리기
    EC_MOVE2(LEECH_SEED),
    // 씨카이저
    EC_POKEMON(WALREIN),
};

const u16 gEasyChatWordsByLetter_KoreanIeung[] = {
    // 아-아
    EC_WORD_UH_OH,
    // 아-앙
    EC_WORD_WAAAH,
    // 아공이
    EC_POKEMON(BAGON),
    // 아기
    EC_WORD_BABE,
    // 아까운
    EC_WORD_CLOSE,
    // 아노딥스
    EC_POKEMON(ANORITH),
    // 아니
    EC_WORD_NOPE,
    // 아니지
    EC_WORD_ISN_T,
    // 아라리
    EC_POKEMON_NATIONAL(EXEGGCUTE),
    // 아래
    EC_WORD_DOWN,
    // 아로마테라피
    EC_MOVE(AROMATHERAPY),
    // 아르바이트
    EC_WORD_CHORES,
    // 아르코
    EC_POKEMON(BELLOSSOM),
    // 아르코
    EC_POKEMON_NATIONAL(BELLOSSOM),
    // 아름다운
    EC_WORD_BEAUTIFUL,
    // 아름다움
    EC_WORD_BEAUTY,
    // 아리아도스
    EC_POKEMON_NATIONAL(ARIADOS),
    // 아마
    EC_WORD_PROBABLY,
    // 아말도
    EC_POKEMON(ARMALDO),
    // 아무도
    EC_WORD_WHO_IS,
    // 아방가르드
    EC_WORD_AVANT_GARDE,
    // 아보
    EC_POKEMON_NATIONAL(EKANS),
    // 아보크
    EC_POKEMON_NATIONAL(ARBOK),
    // 아빠
    EC_WORD_FATHER,
    // 아아
    EC_WORD_HIGHS,
    // 아이
    EC_WORD_KID,
    // 아이돌
    EC_WORD_IDOL,
    // 아이스볼
    EC_MOVE(ICE_BALL),
    // 아이스크
    EC_POKEMON(NINJASK),
    // 아이 아이 써!
    EC_WORD_YES_SIR_EXCL,
    // 아이언테일
    EC_MOVE(IRON_TAIL),
    // 아저씨
    EC_WORD_MAN,
    // 아주 싫어
    EC_WORD_DISLIKES,
    // 아주 좋아
    EC_WORD_ADORE,
    // 아직
    EC_WORD_YET,
    // 아차모
    EC_POKEMON(TORCHIC),
    // 아침
    EC_WORD_MORNING,
    // 아침햇살
    EC_MOVE(MORNING_SUN),
    // 아쿠스타
    EC_POKEMON(STARMIE),
    // 아쿠스타
    EC_POKEMON_NATIONAL(STARMIE),
    // 아픔나누기
    EC_MOVE(PAIN_SPLIT),
    // 아하하
    EC_WORD_AHAHA,
    // 악
    EC_WORD_DARK,
    // 악마의키스
    EC_MOVE(LOVELY_KISS),
    // 악몽
    EC_MOVE(NIGHTMARE),
    // 악취
    EC_WORD_STENCH,
    // 안 나온다
    EC_WORD_DISAPPEAR,
    // 안녕
    EC_WORD_HELLO,
    // 안농
    EC_POKEMON_NATIONAL(UNOWN),
    // 안뇽
    EC_WORD_HOW_DO,
    // 안다
    EC_WORD_KNOWS,
    // 안다리걸기
    EC_MOVE(LOW_KICK),
    // 안 당한다
    EC_WORD_FORGETS,
    // 안돼안돼
    EC_WORD_USELESS,
    // 안 멈추는
    EC_WORD_NON_STOP,
    // 안에
    EC_WORD_INSIDE,
    // 안 온다
    EC_WORD_CAME,
    // 안 한다
    EC_WORD_REFUSE,
    // 안 할래
    EC_WORD_NEVER,
    // 안 해요
    EC_WORD_LIKE,
    // 안했다
    EC_WORD_DIDN_T,
    // 알
    EC_WORD_EGG,
    // 알고
    EC_WORD_KNOW,
    // 알낳기
    EC_MOVE2(SOFT_BOILED),
    // 알통몬
    EC_POKEMON(MACHOP),
    // 알통몬
    EC_POKEMON_NATIONAL(MACHOP),
    // 알폭탄
    EC_MOVE2(EGG_BOMB),
    // 암나이트
    EC_POKEMON_NATIONAL(OMANYTE),
    // 암석봉인
    EC_MOVE(ROCK_TOMB),
    // 암스타
    EC_POKEMON_NATIONAL(OMASTAR),
    // 압정뿌리기
    EC_MOVE2(SPIKES),
    // 앙코르
    EC_MOVE(ENCORE),
    // 앞에
    EC_WORD_FRONT,
    // 애교부리기
    EC_MOVE(CHARM),
    // 애버라스
    EC_POKEMON_NATIONAL(LARVITAR),
    // 앤테이
    EC_POKEMON_NATIONAL(ENTEI),
    // 앱솔
    EC_POKEMON(ABSOL),
    // 야도란
    EC_POKEMON_NATIONAL(SLOWBRO),
    // 야도킹
    EC_POKEMON_NATIONAL(SLOWKING),
    // 야돈
    EC_POKEMON_NATIONAL(SLOWPOKE),
    // 야부엉
    EC_POKEMON_NATIONAL(NOCTOWL),
    // 야호
    EC_WORD_WOW,
    // 야호-
    EC_WORD_YAHOO,
    // 약속
    EC_WORD_PROMISE,
    // 약하다
    EC_WORD_WEAK,
    // 약해져
    EC_WORD_WEAKENED,
    // 얌전한
    EC_WORD_DOCILE,
    // 양보한다
    EC_WORD_TAKE,
    // 얕은
    EC_WORD_SHALLOW,
    // 얘기한다
    EC_WORD_TALK,
    // 어-이
    EC_WORD_OI,
    // 어니부기
    EC_POKEMON_NATIONAL(WARTORTLE),
    // 어둠대신
    EC_POKEMON(SHUPPET),
    // 어디
    EC_WORD_WHERE,
    // 어때?
    EC_WORD_OVER,
    // 어떡하지
    EC_WORD_SHOULD,
    // 어떤가요?
    EC_WORD_ABOUT,
    // 어떻게
    EC_WORD_HOW,
    // 어떻게든
    EC_WORD_SOMETHING,
    // 어른
    EC_WORD_ADULT,
    // 어머
    EC_WORD_OH_QUES,
    // 어버이
    EC_WORD_PARENT,
    // 어서 와
    EC_WORD_WELCOME,
    // 어서 와
    EC_WORD_COME_OVER,
    // 어슬렁
    EC_WORD_WANDERING,
    // 어이
    EC_WORD_OI_OI_OI,
    // 어이쿠
    EC_WORD_OOPS,
    // 어제
    EC_WORD_YESTERDAY,
    // 어쩐지
    EC_WORD_SOMEHOW,
    // 어필
    EC_WORD_APPEAL,
    // 언니
    EC_WORD_LADY,
    // 언제
    EC_WORD_WHEN,
    // 언제나
    EC_WORD_EVER,
    // 언젠가
    EC_WORD_SOMETIME,
    // 얼루기
    EC_POKEMON(SPINDA),
    // 얼어붙은바람
    EC_MOVE(ICY_WIND),
    // 얼음
    EC_WORD_ICE,
    // 얼음귀신
    EC_POKEMON(GLALIE),
    // 엄마
    EC_WORD_MOTHER,
    // 엄청
    EC_WORD_A_LOT,
    // 엄청난힘
    EC_MOVE2(SUPERPOWER),
    // 없는
    EC_WORD_NONE,
    // 없다
    EC_WORD_ABSENT,
    // 없었다
    EC_WORD_NOTHING,
    // 엉망진창
    EC_WORD_DESTROYED,
    // 에나비
    EC_POKEMON(SKITTY),
    // 에는
    EC_WORD_ALTHOUGH,
    // 에도
    EC_WORD_ALSO,
    // 에레브
    EC_POKEMON_NATIONAL(ELECTABUZZ),
    // 에레키드
    EC_POKEMON_NATIONAL(ELEKID),
    // 에메랄드
    EC_WORD_EMERALD,
    // 에브이
    EC_POKEMON_NATIONAL(ESPEON),
    // 에서
    EC_WORD_FROM,
    // 에스퍼
    EC_WORD_PSYCHIC,
    // 에어로블라스트
    EC_MOVE(AEROBLAST),
    // 에어록
    EC_WORD_AIR_LOCK,
    // 에어커터
    EC_MOVE(AIR_CUTTER),
    // 에이팜
    EC_POKEMON_NATIONAL(AIPOM),
    // 에헤헤
    EC_WORD_EHEHE,
    // 에휴
    EC_WORD_LOSING,
    // 엘리게이
    EC_POKEMON_NATIONAL(CROCONAW),
    // 여기
    EC_WORD_HERE,
    // 여름
    EC_WORD_SUMMER,
    // 여보세요
    EC_WORD_LISTENING,
    // 여어
    EC_WORD_HOO_HAH,
    // 여자애
    EC_WORD_GIRL,
    // 여주인공
    EC_WORD_HEROINE,
    // 역린
    EC_MOVE(OUTRAGE),
    // 역시
    EC_WORD_ANY,
    // 역할
    EC_MOVE2(ROLE_PLAY),
    // 연기
    EC_WORD_PRETEND,
    // 연꽃몬
    EC_POKEMON(LOTAD),
    // 연막
    EC_MOVE(SMOKESCREEN),
    // 연속뺨치기
    EC_MOVE(DOUBLE_SLAP),
    // 연속자르기
    EC_MOVE2(FURY_CUTTER),
    // 연속펀치
    EC_MOVE2(COMET_PUNCH),
    // 열풍
    EC_MOVE2(HEAT_WAVE),
    // 염동력
    EC_MOVE2(CONFUSION),
    // 엽록소
    EC_WORD_CHLOROPHYLL,
    // 였다
    EC_WORD_WERE,
    // 였습니다
    EC_WORD_A,
    // 였어
    EC_WORD_WON_T,
    // 영
    EC_WORD_YOUNG,
    // 영원
    EC_WORD_FOREVER,
    // 영치코
    EC_POKEMON(COMBUSKEN),
    // 영화
    EC_WORD_MOVIE,
    // 예
    EC_WORD_YUP,
    // 예-이!
    EC_WORD_YEEHAW_EXCL,
    // 예뻐한다
    EC_WORD_LAVISH,
    // 예스
    EC_WORD_YES,
    // 예정
    EC_WORD_PLANS,
    // 옛날
    EC_WORD_OLDEN,
    // 오-
    EC_WORD_OH,
    // 오 그래
    EC_WORD_OH_YEAH,
    // 오너라!
    EC_WORD_COME_ON,
    // 오늘
    EC_WORD_TODAY,
    // 오뚝군
    EC_POKEMON(BALTOY),
    // 오로라빔
    EC_MOVE(AURORA_BEAM),
    // 오른쪽
    EC_WORD_RIGHT,
    // 오물공격
    EC_MOVE2(SLUDGE),
    // 오물폭탄
    EC_MOVE2(SLUDGE_BOMB),
    // 오버히트
    EC_MOVE(OVERHEAT),
    // 오빠
    EC_WORD_DUDE,
    // 오케이
    EC_WORD_GOT,
    // 오히려
    EC_WORD_RATHER,
    // 온다
    EC_WORD_COME,
    // 올바른
    EC_WORD_CORRECT,
    // 옹골참
    EC_WORD_STURDY,
    // 와아
    EC_WORD_YAY,
    // 와아아!!
    EC_WORD_AWW,
    // 와우
    EC_WORD_WOWEE,
    // 와하하하
    EC_WORD_WAHAHAHA,
    // 왁!!
    EC_WORD_GWAH,
    // 완료
    EC_WORD_COMPLETE,
    // 완벽
    EC_WORD_PERFECTION,
    // 완벽하게
    EC_WORD_PERFECT,
    // 완전히
    EC_WORD_TOTALLY,
    // 왕구리
    EC_POKEMON_NATIONAL(POLITOED),
    // 왕눈해
    EC_POKEMON(TENTACOOL),
    // 왕눈해
    EC_POKEMON_NATIONAL(TENTACOOL),
    // 왕자리
    EC_POKEMON_NATIONAL(YANMA),
    // 왕콘치
    EC_POKEMON(SEAKING),
    // 왕콘치
    EC_POKEMON_NATIONAL(SEAKING),
    // 왜
    EC_WORD_WHY,
    // 외로운
    EC_WORD_LONESOME,
    // 외출
    EC_WORD_TRAVEL,
    // 왼쪽
    EC_WORD_LEFT,
    // 요!
    EC_WORD_WITHOUT,
    // 요-
    EC_WORD_AFTER,
    // 요가랑
    EC_POKEMON(MEDITITE),
    // 요가램
    EC_POKEMON(MEDICHAM),
    // 요가포즈
    EC_MOVE2(MEDITATE),
    // 요우
    EC_WORD_YO,
    // 용돈
    EC_WORD_ALLOWANCE,
    // 용서한다
    EC_WORD_ALLOW,
    // 용서해라
    EC_WORD_FORGET,
    // 용의분노
    EC_MOVE2(DRAGON_RAGE),
    // 용의숨결
    EC_MOVE2(DRAGON_BREATH),
    // 용의춤
    EC_MOVE2(DRAGON_DANCE),
    // 용해액
    EC_MOVE2(ACID),
    // 우르르
    EC_WORD_LOLLING,
    // 우리 편
    EC_WORD_PARTNER,
    // 우린
    EC_WORD_WE_VE,
    // 우오-!
    EC_WORD_WHOAH,
    // 우와-
    EC_WORD_UNBELIEVABLE,
    // 우우
    EC_WORD_URGH,
    // 우워어어-!
    EC_WORD_WROOOAAR_EXCL,
    // 우유마시기
    EC_MOVE2(MILK_DRINK),
    // 우지직우지직
    EC_WORD_BREAK,
    // 우츠동
    EC_POKEMON_NATIONAL(WEEPINBELL),
    // 우츠보트
    EC_POKEMON_NATIONAL(VICTREEBEL),
    // 우파
    EC_POKEMON_NATIONAL(WOOPER),
    // 우후후
    EC_WORD_GIGGLE,
    // 우훗
    EC_WORD_SIGH,
    // 울부짖기
    EC_MOVE2(ROAR),
    // 울음소리
    EC_MOVE2(GROWL),
    // 웅크리기
    EC_MOVE2(DEFENSE_CURL),
    // 원념
    EC_MOVE(GRUDGE),
    // 원시의힘
    EC_MOVE(ANCIENT_POWER),
    // 원한
    EC_MOVE(SPITE),
    // 월드
    EC_WORD_WORLD,
    // 월요일
    EC_WORD_MONDAY,
    // 웨더볼
    EC_MOVE(WEATHER_BALL),
    // 위
    EC_WORD_UP,
    // 위에
    EC_WORD_ABOVE,
    // 위협
    EC_WORD_INTIMIDATE,
    // 윈디
    EC_POKEMON_NATIONAL(ARCANINE),
    // 유감
    EC_WORD_DISAPPOINT,
    // 유니언
    EC_WORD_UNION,
    // 유연
    EC_WORD_LIMBER,
    // 유치원
    EC_WORD_KINDERGARTEN,
    // 유행
    EC_WORD_TRENDY,
    // 윤겔라
    EC_POKEMON(KADABRA),
    // 윤겔라
    EC_POKEMON_NATIONAL(KADABRA),
    // 으-음
    EC_WORD_HMM,
    // 으드득으드득
    EC_WORD_AWESOME,
    // 으아-!
    EC_WORD_ARRGH,
    // 으아앙
    EC_WORD_CRIES,
    // 으앙
    EC_WORD_CRY,
    // 으으음
    EC_WORD_MMM,
    // 으음
    EC_WORD_LAY,
    // 으흐흐
    EC_WORD_MUFUFU,
    // 은빛바람
    EC_MOVE(SILVER_WIND),
    // 은혜갚기
    EC_MOVE(RETURN),
    // 음악
    EC_WORD_MUSIC,
    // 음침한
    EC_WORD_SHADY,
    // 의
    EC_WORD_OF,
    // 의기양양
    EC_WORD_VITAL_SPIRIT,
    // 의욕
    EC_WORD_HUSTLE,
    // 이
    EC_WORD_BESIDE,
    // 이거야
    EC_WORD_THIS_IS_IT_EXCL,
    // 이건
    EC_WORD_THESE_WERE,
    // 이겨서
    EC_WORD_WHEN_I_WIN,
    // 이겼다
    EC_WORD_WON,
    // 이글이글
    EC_WORD_FIERY,
    // 이기게 해줘
    EC_WORD_LET_ME_WIN,
    // 이기면
    EC_WORD_IF_I_WIN,
    // 이긴다
    EC_WORD_WIN,
    // 이길 수 없다
    EC_WORD_CAN_T_WIN,
    // 이길 수 있다
    EC_WORD_CAN_WIN,
    // 이다
    EC_WORD_AM,
    // 이런
    EC_WORD_THIS,
    // 이름
    EC_WORD_NAME,
    // 이모
    EC_WORD_AUNT,
    // 이 몸
    EC_WORD_ME,
    // 이번에
    EC_WORD_ANOTHER,
    // 이벤트
    EC_WORD_EVENTS,
    // 이봐
    EC_WORD_HEY_THERE,
    // 이봐
    EC_WORD_UH_HUH,
    // 이브이
    EC_POKEMON_NATIONAL(EEVEE),
    // 이상
    EC_WORD_WEIRD,
    // 이상한
    EC_WORD_MYSTERY,
    // 이상한비늘
    EC_WORD_MARVEL_SCALE,
    // 이상한빛
    EC_MOVE(CONFUSE_RAY),
    // 이상해꽃
    EC_POKEMON_NATIONAL(VENUSAUR),
    // 이상해씨
    EC_POKEMON_NATIONAL(BULBASAUR),
    // 이상해풀
    EC_POKEMON_NATIONAL(IVYSAUR),
    // 이야
    EC_WORD_HIYAH,
    // 이야-
    EC_WORD_YEAH,
    // 이야기
    EC_WORD_TALKING,
    // 이전에
    EC_WORD_EARLIER,
    // 이제
    EC_WORD_ALMOST,
    // 이지
    EC_WORD_CAN_T,
    // 이쪽
    EC_WORD_THESE,
    // 이판사판태클
    EC_MOVE(DOUBLE_EDGE),
    // 이하
    EC_WORD_LESS,
    // 이해
    EC_WORD_UNDERSTANDS,
    // 이해한다
    EC_WORD_UNDERSTAND,
    // 익사이트
    EC_WORD_EXCITE,
    // 인기
    EC_WORD_POPULAR,
    // 인분
    EC_WORD_SHIELD_DUST,
    // 인정 못 한다
    EC_WORD_REJECT,
    // 인정한다
    EC_WORD_ACCEPT,
    // 일
    EC_WORD_WORK,
    // 일까?
    EC_WORD_IS_IT_QUES,
    // 일단
    EC_WORD_FOR_NOW,
    // 일어난다
    EC_WORD_WAKES_UP,
    // 일어남
    EC_WORD_WAKE_UP,
    // 일요일
    EC_WORD_SUNDAY,
    // 일지도
    EC_WORD_BE,
    // 일찍기상
    EC_WORD_EARLY_BIRD,
    // 임다
    EC_WORD_WHOM,
    // 입니까?
    EC_WORD_OK_QUES,
    // 입니다
    EC_WORD_THERE,
    // 입니다-!
    EC_WORD_HAVEN_T,
    // 입치트
    EC_POKEMON(MAWILE),
    // 있고
    EC_WORD_APPROVED,
    // 있다
    EC_WORD_HAS,
    // 있다면
    EC_WORD_FANTASTIC,
    // 있습니다
    EC_WORD_WILL_BE_HERE,
    // 있었다
    EC_WORD_EXISTS,
    // 있잖아
    EC_WORD_LISTEN,
    // 있지
    EC_WORD_HUH_QUES,
    // 잉어킹
    EC_POKEMON(MAGIKARP),
    // 잉어킹
    EC_POKEMON_NATIONAL(MAGIKARP),
    // 잊어
    EC_WORD_FAINTED,
    // 잎날가르기
    EC_MOVE2(RAZOR_LEAF),
    // 잎새코
    EC_POKEMON(NUZLEAF),
};

const u16 gEasyChatWordsByLetter_KoreanJieut[] = {
    // 자
    EC_WORD_MISS,
    // 자 그럼
    EC_WORD_HIT,
    // 자기암시
    EC_MOVE(PSYCH_UP),
    // 자랑
    EC_WORD_BRAG,
    // 자력
    EC_WORD_MAGNET_PULL,
    // 자신
    EC_WORD_MYSELF,
    // 자신에게
    EC_WORD_TO_ME,
    // 자신은
    EC_WORD_I_AM,
    // 자신을
    EC_WORD_I_VE,
    // 자신의
    EC_WORD_MY,
    // 자신이
    EC_WORD_I_WAS,
    // 자연의힘
    EC_MOVE(NATURE_POWER),
    // 자연회복
    EC_WORD_NATURAL_CURE,
    // 자전거
    EC_WORD_BIKE,
    // 자폭
    EC_MOVE(SELF_DESTRUCT),
    // 작아지기
    EC_MOVE2(MINIMIZE),
    // 작은
    EC_WORD_SMALL,
    // 작은아버지
    EC_WORD_UNCLE,
    // 작전
    EC_WORD_STRATEGY,
    // 잔다
    EC_WORD_SLEEP,
    // 잔비
    EC_WORD_DRIZZLE,
    // 잖아
    EC_WORD_ISN_T_IT_QUES,
    // 잘
    EC_WORD_WELL,
    // 잘 가
    EC_WORD_GOOD_BYE,
    // 잘 부탁해
    EC_WORD_COUNT_ON,
    // 잘 부탁해
    EC_WORD_KTHX_BYE,
    // 잘하는
    EC_WORD_SKILLED,
    // 잠깐
    EC_WORD_A_TINY_BIT,
    // 잠꼬대
    EC_MOVE2(SLEEP_TALK),
    // 잠듦
    EC_WORD_SLEPT,
    // 잠만보
    EC_POKEMON_NATIONAL(SNORLAX),
    // 잠자기
    EC_MOVE2(REST),
    // 잠재파워
    EC_MOVE2(HIDDEN_POWER),
    // 잡지
    EC_WORD_MAGAZINE,
    // 장난감
    EC_WORD_TOYS,
    // 장크로다일
    EC_POKEMON_NATIONAL(FERALIGATR),
    // 재능
    EC_WORD_TALENT,
    // 재미있게
    EC_WORD_ENTERTAINING,
    // 재미있는
    EC_WORD_AMUSING,
    // 잼잼펀치
    EC_MOVE2(DIZZY_PUNCH),
    // 쟝고
    EC_POKEMON(ZANGOOSE),
    // 저
    EC_WORD_UM,
    // 저거야
    EC_WORD_REAR,
    // 저건
    EC_WORD_THINGS,
    // 저기
    EC_WORD_HEY_QUES,
    // 저기-
    EC_WORD_SINCE,
    // 저는
    EC_WORD_SHE_IS,
    // 저런
    EC_WORD_OH_DEAR,
    // 저를
    EC_WORD_SOME,
    // 저리가루
    EC_MOVE(STUN_SPORE),
    // 저수
    EC_WORD_WATER_ABSORB,
    // 저의
    EC_WORD_HERS,
    // 저주
    EC_MOVE2(CURSE),
    // 저질렀다
    EC_WORD_DARN,
    // 저쪽
    EC_WORD_LOWS,
    // 전격파
    EC_MOVE2(SHOCK_WAVE),
    // 전광석화
    EC_MOVE2(QUICK_ATTACK),
    // 전기
    EC_WORD_ELECTRIC,
    // 전기쇼크
    EC_MOVE2(THUNDER_SHOCK),
    // 전기자석파
    EC_MOVE2(THUNDER_WAVE),
    // 전당등록
    EC_WORD_HALL_OF_FAME,
    // 전룡
    EC_POKEMON_NATIONAL(AMPHAROS),
    // 전설
    EC_WORD_LEGEND,
    // 전자포
    EC_MOVE2(ZAP_CANNON),
    // 전철
    EC_WORD_LOCOMOTIVE,
    // 전투무장
    EC_WORD_BATTLE_ARMOR,
    // 전혀
    EC_WORD_HOPELESS,
    // 전화
    EC_WORD_PHONE,
    // 절대로
    EC_WORD_ABSOLUTELY,
    // 절대영도
    EC_MOVE(SHEER_COLD),
    // 점점
    EC_WORD_INCREASING,
    // 점착
    EC_WORD_STICKY_HOLD,
    // 점토도리
    EC_POKEMON(CLAYDOL),
    // 점프킥
    EC_MOVE2(JUMP_KICK),
    // 정말
    EC_WORD_REALLY,
    // 정보
    EC_WORD_INFORMATION,
    // 정신력
    EC_WORD_INNER_FOCUS,
    // 정신차리기
    EC_MOVE(SMELLING_SALTS),
    // 정전기
    EC_WORD_STATIC,
    // 정했다
    EC_WORD_DECIDED,
    // 젖은접시
    EC_WORD_RAIN_DISH,
    // 제가
    EC_WORD_SHE_WAS,
    // 제게
    EC_WORD_TO_HER,
    // 제법
    EC_WORD_PRETTY,
    // 제비반환
    EC_MOVE2(AERIAL_ACE),
    // 젠장
    EC_WORD_BAD,
    // 져서
    EC_WORD_LOST,
    // 졌다
    EC_WORD_GIVE_UP,
    // 조가비갑옷
    EC_WORD_SHELL_ARMOR,
    // 조금은
    EC_WORD_A_LITTLE,
    // 조만간
    EC_WORD_NOW,
    // 조수
    EC_MOVE2(ASSIST),
    // 조이기
    EC_MOVE(BIND),
    // 좀 더
    EC_WORD_MORE,
    // 종료
    EC_WORD_FINISH,
    // 좋-아
    EC_WORD_OH_KAY,
    // 좋아
    EC_WORD_LIKES,
    // 좋아!
    EC_WORD_OKAY,
    // 좋은
    EC_WORD_GOOD,
    // 죄송합니다
    EC_WORD_EXCUSE_ME,
    // 주뱃
    EC_POKEMON(ZUBAT),
    // 주뱃
    EC_POKEMON_NATIONAL(ZUBAT),
    // 주세요
    EC_WORD_GIVE_ME,
    // 주인공
    EC_WORD_HERO,
    // 죽기살기
    EC_MOVE(ENDEAVOR),
    // 준다
    EC_WORD_GIVE,
    // 줄줄
    EC_WORD_DROOLING,
    // 중요
    EC_WORD_CARE,
    // 쥬레곤
    EC_POKEMON_NATIONAL(DEWGONG),
    // 쥬쥬
    EC_POKEMON_NATIONAL(SEEL),
    // 쥬피썬더
    EC_POKEMON_NATIONAL(JOLTEON),
    // 즐겁다
    EC_WORD_ENJOY,
    // 지각
    EC_WORD_LATE,
    // 지구던지기
    EC_MOVE2(SEISMIC_TOSS),
    // 지그제구리
    EC_POKEMON(ZIGZAGOON),
    // 지금
    EC_WORD_CURRENT,
    // 지라치
    EC_POKEMON(JIRACHI),
    // 지루함
    EC_WORD_BORING,
    // 지면
    EC_WORD_IF_I_LOSE,
    // 지옥의바퀴
    EC_MOVE(SUBMISSION),
    // 지진
    EC_MOVE(EARTHQUAKE),
    // 직구리
    EC_POKEMON(LINOONE),
    // 진다
    EC_WORD_LOSE,
    // 진심
    EC_WORD_SERIOUS,
    // 진주몽
    EC_POKEMON(CLAMPERL),
    // 진지하게
    EC_WORD_SERIOUSLY,
    // 진짜로
    EC_WORD_TRULY,
    // 진화
    EC_WORD_EVOLUTION,
    // 진화해
    EC_WORD_EVOLVE,
    // 진흙뿌리기
    EC_MOVE2(MUD_SLAP),
    // 질뻐기
    EC_POKEMON(MUK),
    // 질뻐기
    EC_POKEMON_NATIONAL(MUK),
    // 질퍽이
    EC_POKEMON(GRIMER),
    // 질퍽이
    EC_POKEMON_NATIONAL(GRIMER),
    // 집
    EC_WORD_HOME,
    // 집게해머
    EC_MOVE(CRABHAMMER),
    // 집단구타
    EC_MOVE2(BEAT_UP),
    // 집 보기
    EC_WORD_STAY_AT_HOME,
    // 짓궂다
    EC_WORD_MEAN,
    // 짓밟기
    EC_MOVE2(STOMP),
};

const u16 gEasyChatWordsByLetter_KoreanSsangJieut[] = {
    // 째려보기
    EC_MOVE2(LEER),
    // 쪼기
    EC_MOVE2(PECK),
    // 찌리리공
    EC_EMPTY_WORD, 2,
    EC_POKEMON(VOLTORB),
    EC_POKEMON_NATIONAL(VOLTORB),
    // 찝기
    EC_MOVE2(VISE_GRIP),
};

const u16 gEasyChatWordsByLetter_KoreanChieut[] = {
    // 착한
    EC_WORD_KIND,
    // 참기
    EC_MOVE(BIDE),
    // 참음
    EC_WORD_TRUST,
    // 찾는다
    EC_WORD_SEEK,
    // 찾음
    EC_WORD_SEARCH,
    // 채널
    EC_WORD_CHANNEL,
    // 책
    EC_WORD_BOOK,
    // 천사의키스
    EC_MOVE2(SWEET_KISS),
    // 천재
    EC_WORD_GENIUS,
    // 천하장사
    EC_WORD_HUGE_POWER,
    // 철벽
    EC_MOVE2(IRON_DEFENSE),
    // 쳇
    EC_WORD_TCH,
    // 초라기
    EC_EMPTY_WORD, 2,
    EC_POKEMON(CHINCHOU),
    EC_POKEMON_NATIONAL(CHINCHOU),
    // 초음파
    EC_MOVE2(SUPERSONIC),
    // 촌스럽다
    EC_WORD_LOSER,
    // 총어
    EC_POKEMON_NATIONAL(REMORAID),
    // 최고
    EC_WORD_NO_1,
    // 최면술
    EC_MOVE(HYPNOSIS),
    // 최신
    EC_WORD_HIP_AND,
    // 추억의선물
    EC_MOVE(MEMENTO),
    // 추운
    EC_WORD_COLD,
    // 추천
    EC_WORD_RECOMMEND,
    // 축전
    EC_WORD_VOLT_ABSORB,
    // 축제
    EC_WORD_FESTIVAL,
    // 축하해
    EC_WORD_CONGRATS,
    // 충분
    EC_WORD_ENOUGH,
    // 충전
    EC_MOVE(CHARGE),
    // 취미
    EC_WORD_HOBBY,
    // 치렁
    EC_POKEMON(CHIMECHO),
    // 치료방울
    EC_MOVE(HEAL_BELL),
    // 치코리타
    EC_POKEMON_NATIONAL(CHIKORITA),
    // 친구
    EC_WORD_FRIEND,
    // 칠색조
    EC_POKEMON_NATIONAL(HO_OH),
    // 침바루
    EC_POKEMON_NATIONAL(QWILFISH),
    // 칭찬
    EC_WORD_PRAISE,
};

const u16 gEasyChatWordsByLetter_KoreanKieuk[] = {
    // 카드
    EC_WORD_CARDS,
    // 카메라
    EC_WORD_CAMERA,
    // 카스쿤
    EC_POKEMON(CASCOON),
    // 카운터
    EC_MOVE(COUNTER),
    // 카포에라
    EC_POKEMON_NATIONAL(HITMONTOP),
    // 칼등치기
    EC_MOVE2(FALSE_SWIPE),
    // 칼바람
    EC_MOVE(RAZOR_WIND),
    // 칼춤
    EC_MOVE2(SWORDS_DANCE),
    // 캐스퐁
    EC_POKEMON(CASTFORM),
    // 캐이시
    EC_EMPTY_WORD, 2,
    EC_POKEMON(ABRA),
    EC_POKEMON_NATIONAL(ABRA),
    // 캐터피
    EC_POKEMON_NATIONAL(CATERPIE),
    // 캥카
    EC_POKEMON_NATIONAL(KANGASKHAN),
    // 커플
    EC_WORD_COUPLE,
    // 컬렉션
    EC_WORD_COLLECTION,
    // 켁
    EC_WORD_WAY,
    // 켄타로스
    EC_POKEMON_NATIONAL(TAUROS),
    // 켈리몬
    EC_POKEMON(KECLEON),
    // 코골기
    EC_MOVE(SNORE),
    // 코리갑
    EC_EMPTY_WORD, 2,
    EC_POKEMON(DONPHAN),
    EC_POKEMON_NATIONAL(DONPHAN),
    // 코멧펀치
    EC_MOVE(METEOR_MASH),
    // 코뿌리
    EC_EMPTY_WORD, 2,
    EC_POKEMON(RHYDON),
    EC_POKEMON_NATIONAL(RHYDON),
    // 코산호
    EC_EMPTY_WORD, 2,
    EC_POKEMON(CORSOLA),
    EC_POKEMON_NATIONAL(CORSOLA),
    // 코스믹파워
    EC_MOVE(COSMIC_POWER),
    // 코일
    EC_EMPTY_WORD, 2,
    EC_POKEMON(MAGNEMITE),
    EC_POKEMON_NATIONAL(MAGNEMITE),
    // 코코리
    EC_EMPTY_WORD, 2,
    EC_POKEMON(PHANPY),
    EC_POKEMON_NATIONAL(PHANPY),
    // 코코파스
    EC_POKEMON(NOSEPASS),
    // 코터스
    EC_POKEMON(TORKOAL),
    // 콘치
    EC_EMPTY_WORD, 2,
    EC_POKEMON(GOLDEEN),
    EC_POKEMON_NATIONAL(GOLDEEN),
    // 콘테스트
    EC_WORD_CONTEST,
    // 콘팡
    EC_POKEMON_NATIONAL(VENONAT),
    // 쾌청
    EC_MOVE2(SUNNY_DAY),
    // 쿵쿵
    EC_WORD_THICK,
    // 크랩
    EC_POKEMON_NATIONAL(KRABBY),
    // 크러시
    EC_WORD_CRUSH,
    // 크로뱃
    EC_EMPTY_WORD, 2,
    EC_POKEMON(CROBAT),
    EC_POKEMON_NATIONAL(CROBAT),
    // 크로스촙
    EC_MOVE(CROSS_CHOP),
    // 크르르르
    EC_WORD_GRAAAH,
    // 큰
    EC_WORD_BIG,
    // 큰일
    EC_WORD_DISASTER,
    // 큰일이다
    EC_WORD_DANGER,
    // 클리어바디
    EC_WORD_CLEAR_BODY,
    // 키링키
    EC_EMPTY_WORD, 2,
    EC_POKEMON(GIRAFARIG),
    EC_POKEMON_NATIONAL(GIRAFARIG),
    // 킬리아
    EC_POKEMON(KIRLIA),
    // 킹드라
    EC_EMPTY_WORD, 2,
    EC_POKEMON(KINGDRA),
    EC_POKEMON_NATIONAL(KINGDRA),
    // 킹크랩
    EC_POKEMON_NATIONAL(KINGLER),
};

const u16 gEasyChatWordsByLetter_KoreanTieut[] = {
    // 타오르는불꽃
    EC_WORD_FLASH_FIRE,
    // 타워
    EC_WORD_TOWER,
    // 타입
    EC_WORD_TYPE,
    // 탁류
    EC_MOVE2(MUDDY_WATER),
    // 탁쳐서떨구기
    EC_MOVE2(KNOCK_OFF),
    // 탈피
    EC_WORD_SHED_SKIN,
    // 탐내다
    EC_MOVE2(COVET),
    // 탕구리
    EC_POKEMON_NATIONAL(CUBONE),
    // 태권당수
    EC_MOVE(KARATE_CHOP),
    // 터무니없는
    EC_WORD_PREPOSTEROUS,
    // 텅구리
    EC_POKEMON_NATIONAL(MAROWAK),
    // 테오키스
    EC_POKEMON(DEOXYS),
    // 테일로
    EC_POKEMON(TAILLOW),
    // 텍스처
    EC_MOVE2(CONVERSION),
    // 텍스처2
    EC_MOVE2(CONVERSION_2),
    // 토게틱
    EC_POKEMON_NATIONAL(TOGETIC),
    // 토게피
    EC_POKEMON_NATIONAL(TOGEPI),
    // 토요일
    EC_WORD_SATURDAY,
    // 토중몬
    EC_POKEMON(NINCADA),
    // 토해내기
    EC_MOVE2(SPIT_UP),
    // 톱
    EC_WORD_TOP,
    // 톱치
    EC_POKEMON(TRAPINCH),
    // 통신
    EC_WORD_LINK,
    // 통통코
    EC_POKEMON_NATIONAL(HOPPIP),
    // 투구
    EC_POKEMON_NATIONAL(KABUTO),
    // 투구푸스
    EC_POKEMON_NATIONAL(KABUTOPS),
    // 튀어오르기
    EC_MOVE2(SPLASH),
    // 트라이어택
    EC_MOVE2(TRI_ATTACK),
    // 트레이너
    EC_WORD_TRAINER,
    // 트레이스
    EC_WORD_TRACE,
    // 트렌디
    EC_WORD_HAPPENING,
    // 트로피우스
    EC_POKEMON(TROPIUS),
    // 트리플킥
    EC_MOVE2(TRIPLE_KICK),
    // 트릭
    EC_MOVE2(TRICK),
    // 트집
    EC_MOVE(TORMENT),
    // 특기
    EC_WORD_SKILL,
    // 특성
    EC_WORD_SP_ABILITY,
};

const u16 gEasyChatWordsByLetter_KoreanPieup[] = {
    // 파괴광선
    EC_MOVE2(HYPER_BEAM),
    // 파도타기
    EC_MOVE2(SURF),
    // 파라섹트
    EC_POKEMON_NATIONAL(PARASECT),
    // 파라스
    EC_POKEMON_NATIONAL(PARAS),
    // 파르셀
    EC_POKEMON_NATIONAL(CLOYSTER),
    // 파멸의소원
    EC_MOVE2(DOOM_DESIRE),
    // 파비코
    EC_POKEMON(SWABLU),
    // 파비코리
    EC_POKEMON(ALTARIA),
    // 파오리
    EC_POKEMON_NATIONAL(FARFETCHD),
    // 파이리
    EC_POKEMON_NATIONAL(CHARMANDER),
    // 파이어
    EC_WORD_FLAME,
    // 파이어
    EC_POKEMON_NATIONAL(MOLTRES),
    // 파이트
    EC_WORD_FIGHT,
    // 파이팅
    EC_WORD_HASSLE,
    // 파티
    EC_WORD_PARTY,
    // 판별
    EC_MOVE2(DETECT),
    // 패리퍼
    EC_POKEMON(PELIPPER),
    // 패배
    EC_WORD_LOSS,
    // 패션
    EC_WORD_FASHION,
    // 팬텀
    EC_POKEMON_NATIONAL(GENGAR),
    // 페르시온
    EC_POKEMON_NATIONAL(PERSIAN),
    // 페이검
    EC_POKEMON_NATIONAL(SPINARAK),
    // 편의점
    EC_WORD_STORE,
    // 편지
    EC_WORD_LETTER,
    // 포기
    EC_WORD_CONCEDE,
    // 포니타
    EC_POKEMON_NATIONAL(PONYTA),
    // 포이즌테일
    EC_MOVE2(POISON_TAIL),
    // 포인트
    EC_WORD_POINTS,
    // 포자
    EC_WORD_EFFECT_SPORE,
    // 포챠나
    EC_POKEMON(POOCHYENA),
    // 포켓기어
    EC_WORD_POKENAV,
    // 포켓러스
    EC_WORD_PKRS,
    // 포켓몬
    EC_WORD_POKEMON,
    // 포켓몬 겟
    EC_WORD_GET,
    // 포켓몬 도감
    EC_WORD_POKEDEX,
    // 포켓몬스낵
    EC_WORD_POKEBLOCK,
    // 포푸니
    EC_POKEMON_NATIONAL(SNEASEL),
    // 폭발펀치
    EC_MOVE2(DYNAMIC_PUNCH),
    // 폭음룡
    EC_POKEMON(EXPLOUD),
    // 폭타
    EC_POKEMON(CAMERUPT),
    // 폭포오르기
    EC_MOVE2(WATERFALL),
    // 폴리곤
    EC_POKEMON_NATIONAL(PORYGON),
    // 폴리곤2
    EC_POKEMON_NATIONAL(PORYGON2),
    // 푸린
    EC_EMPTY_WORD, 2,
    EC_POKEMON(JIGGLYPUFF),
    EC_POKEMON_NATIONAL(JIGGLYPUFF),
    // 푸크린
    EC_EMPTY_WORD, 2,
    EC_POKEMON(WIGGLYTUFF),
    EC_POKEMON_NATIONAL(WIGGLYTUFF),
    // 푸푸린
    EC_EMPTY_WORD, 2,
    EC_POKEMON(IGGLYBUFF),
    EC_POKEMON_NATIONAL(IGGLYBUFF),
    // 풀
    EC_WORD_GRASS,
    // 풀베기
    EC_MOVE(CUT),
    // 풀피리
    EC_MOVE(GRASS_WHISTLE),
    // 풉
    EC_WORD_LOL,
    // 프런티어
    EC_WORD_FRONTIER,
    // 프레셔
    EC_WORD_PRESSURE,
    // 프레젠트
    EC_MOVE2(PRESENT),
    // 프리져
    EC_POKEMON_NATIONAL(ARTICUNO),
    // 프테라
    EC_POKEMON_NATIONAL(AERODACTYL),
    // 플라이곤
    EC_POKEMON(FLYGON),
    // 플래시
    EC_MOVE2(FLASH),
    // 플러스
    EC_WORD_PLUS,
    // 플러시
    EC_POKEMON(PLUSLE),
    // 피곤한
    EC_WORD_TIRED,
    // 피그점프
    EC_POKEMON(SPOINK),
    // 피그킹
    EC_POKEMON(GRUMPIG),
    // 피뢰침
    EC_WORD_LIGHTNINGROD,
    // 피버
    EC_WORD_FEVER,
    // 피죤
    EC_POKEMON_NATIONAL(PIDGEOTTO),
    // 피죤투
    EC_POKEMON_NATIONAL(PIDGEOT),
    // 피츄
    EC_EMPTY_WORD, 2,
    EC_POKEMON(PICHU),
    EC_POKEMON_NATIONAL(PICHU),
    // 피카츄
    EC_EMPTY_WORD, 2,
    EC_POKEMON(PIKACHU),
    EC_POKEMON_NATIONAL(PIKACHU),
    // 피콘
    EC_POKEMON_NATIONAL(PINECO),
    // 픽시
    EC_POKEMON_NATIONAL(CLEFABLE),
    // 픽업
    EC_WORD_PICKUP,
    // 필살앞니
    EC_MOVE2(HYPER_FANG),
    // 필요하다
    EC_WORD_NEED,
};

const u16 gEasyChatWordsByLetter_KoreanHieut[] = {
    // 하고 있다
    EC_WORD_HAVE,
    // 하곤 한다
    EC_WORD_LIKELY_TO,
    // 하는걸
    EC_WORD_DOESN_T,
    // 하는구나
    EC_WORD_BEFORE,
    // 하는 중
    EC_WORD_WHICH,
    // 하늘의은총
    EC_WORD_SERENE_GRACE,
    // 하도록 해
    EC_WORD_ALL,
    // 하드플랜트
    EC_MOVE2(FRENZY_PLANT),
    // 하루
    EC_WORD_DAY,
    // 하리뭉
    EC_POKEMON(HARIYAMA),
    // 하면
    EC_WORD_IS,
    // 하세요
    EC_WORD_GO_AHEAD,
    // 하아하아
    EC_WORD_THIN,
    // 하얀연기
    EC_WORD_WHITE_SMOKE,
    // 하이
    EC_WORD_HI,
    // 하이드로캐논
    EC_MOVE2(HYDRO_CANNON),
    // 하이드로펌프
    EC_MOVE2(HYDRO_PUMP),
    // 하이퍼
    EC_WORD_HYPER,
    // 하이퍼보이스
    EC_MOVE2(HYPER_VOICE),
    // 하자
    EC_WORD_LET_S,
    // 하자고!
    EC_WORD_OTHER,
    // 하지만
    EC_WORD_BUT,
    // 하품
    EC_MOVE(YAWN),
    // 하하하
    EC_WORD_HAHAHA,
    // 학교
    EC_WORD_SCHOOL,
    // 학원
    EC_WORD_LESSONS,
    // 한가한
    EC_WORD_BORED,
    // 한다
    EC_WORD_SLIDE,
    // 한답니다
    EC_WORD_TOO,
    // 한 방에!
    EC_WORD_1_HIT_KO_EXCL,
    // 한 번도
    EC_WORD_LAST,
    // 할게
    EC_WORD_ONCE,
    // 할게요
    EC_WORD_WHILE,
    // 할까
    EC_WORD_WONDER,
    // 할래
    EC_WORD_GETS,
    // 할머니
    EC_WORD_GRANDMOTHER,
    // 할 생각
    EC_WORD_WASN_T,
    // 할 수 없다
    EC_WORD_COULDN_T,
    // 할 수 있다
    EC_WORD_CAPABLE,
    // 할아버지
    EC_WORD_GRANDFATHER,
    // 할짝할짝
    EC_WORD_RIPPED,
    // 할퀴기
    EC_MOVE2(SCRATCH),
    // 핥기
    EC_MOVE(LICK),
    // 합니다
    EC_WORD_WOULD,
    // 합니다-!
    EC_WORD_IN,
    // 핫삼
    EC_POKEMON_NATIONAL(SCIZOR),
    // 항복
    EC_WORD_SURRENDER,
    // 항상
    EC_WORD_ALWAYS,
    // 해감액
    EC_WORD_LIQUID_OOZE,
    // 해골몽
    EC_POKEMON(DUSKULL),
    // 해냈다
    EC_WORD_DONE,
    // 해너츠
    EC_POKEMON_NATIONAL(SUNKERN),
    // 해루미
    EC_POKEMON_NATIONAL(SUNFLORA),
    // 해수스파우팅
    EC_MOVE(WATER_SPOUT),
    // 해요
    EC_WORD_OUT,
    // 해피너스
    EC_POKEMON_NATIONAL(BLISSEY),
    // 했습니다
    EC_WORD_ANYWHERE,
    // 행복
    EC_WORD_JOY,
    // 헌테일
    EC_POKEMON(HUNTAIL),
    // 헤라크로스
    EC_POKEMON(HERACROSS),
    // 헤라크로스
    EC_POKEMON_NATIONAL(HERACROSS),
    // 헤롱헤롱
    EC_MOVE2(ATTRACT),
    // 헤롱헤롱바디
    EC_WORD_CUTE_CHARM,
    // 헤이
    EC_WORD_HEY,
    // 헤헤
    EC_WORD_HEH,
    // 헤헤헤
    EC_WORD_HEHEHE,
    // 헬가
    EC_POKEMON_NATIONAL(HOUNDOOM),
    // 형
    EC_WORD_BROTHER,
    // 형제
    EC_WORD_SIBLINGS,
    // 호드득
    EC_WORD_SCATTER,
    // 호호호
    EC_WORD_HOHOHO,
    // 혼란스러운
    EC_WORD_CONFUSED,
    // 혼자서
    EC_WORD_ALONE,
    // 홍수몬
    EC_POKEMON_NATIONAL(HITMONCHAN),
    // 화남
    EC_WORD_ANGERS,
    // 화염바퀴
    EC_MOVE(FLAME_WHEEL),
    // 화염방사
    EC_MOVE(FLAMETHROWER),
    // 화요일
    EC_WORD_TUESDAY,
    // 화풀이
    EC_MOVE2(FRUSTRATION),
    // 환상빔
    EC_MOVE(PSYBEAM),
    // 활약
    EC_WORD_WORKS,
    // 활약한다
    EC_WORD_WORKING,
    // 회오리
    EC_MOVE2(TWISTER),
    // 회오리불꽃
    EC_MOVE2(FIRE_SPIN),
    // 회전부리
    EC_MOVE2(DRILL_PECK),
    // 후딘
    EC_POKEMON(ALAKAZAM),
    // 후딘
    EC_POKEMON_NATIONAL(ALAKAZAM),
    // 후후후
    EC_WORD_FUFUFU,
    // 훌륭한
    EC_WORD_IMPORTANT,
    // 훌륭해
    EC_WORD_EXCELLENT,
    // 훌쩍훌쩍
    EC_WORD_SADLY,
    // 휘감기
    EC_MOVE(CONSTRICT),
    // 휴-
    EC_WORD_HAH,
    // 휴일
    EC_WORD_HOLIDAY,
    // 흉내내기
    EC_MOVE2(MIMIC),
    // 흐느적흐느적
    EC_WORD_WIMPY,
    // 흐흥
    EC_WORD_SNORT,
    // 흑안개
    EC_MOVE(HAZE),
    // 흔들흔들
    EC_WORD_WOBBLY,
    // 흔들흔들댄스
    EC_MOVE2(TEETER_DANCE),
    // 흙놀이
    EC_MOVE2(MUD_SPORT),
    // 흡반
    EC_WORD_SUCTION_CUPS,
    // 흡수
    EC_MOVE(ABSORB),
    // 흡혈
    EC_MOVE(LEECH_LIFE),
    // 흥
    EC_WORD_HUMPH,
    // 희귀하다
    EC_WORD_RARE,
    // 희망사항
    EC_MOVE2(WISH),
    // 흰안개
    EC_MOVE(MIST),
    // 히든카드
    EC_WORD_TRUMP_CARD,
    // 히익-
    EC_WORD_AIYEEH,
    // 힘
    EC_WORD_POWER,
    // 힘껏치기
    EC_MOVE2(SLAM),
    // 힘껏펀치
    EC_MOVE(FOCUS_PUNCH),
    // 힘내서
    EC_WORD_TRY,
    // 힘낼래
    EC_WORD_TRIES,
};

const u16 gEasyChatWordsByLetter_KoreanUnused20[] = {
    EC_EMPTY_WORD,
};

const u16 gEasyChatWordsByLetter_KoreanUnused21[] = {
    EC_EMPTY_WORD,
};

const u16 gEasyChatWordsByLetter_KoreanUnused22[] = {
    EC_EMPTY_WORD,
};

const u16 gEasyChatWordsByLetter_KoreanUnused23[] = {
    EC_EMPTY_WORD,
};

const u16 gEasyChatWordsByLetter_KoreanUnused24[] = {
    EC_EMPTY_WORD,
};

const u16 gEasyChatWordsByLetter_KoreanUnused25[] = {
    EC_EMPTY_WORD,
};

const u16 gEasyChatWordsByLetter_KoreanUnused26[] = {
    EC_EMPTY_WORD,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseHi[] = {
    EC_POKEMON_NATIONAL(CLEFFA),
    EC_WORD_AIYEEH,
    EC_POKEMON_NATIONAL(WEEDLE),
    EC_WORD_HERO,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(PIKACHU),
    EC_POKEMON(PIKACHU),
    EC_MOVE2(LIGHT_SCREEN),
    EC_WORD_SHINE,
    EC_WORD_LOW,
    EC_POKEMON_NATIONAL(CLEFABLE),
    EC_WORD_FLYING,
    EC_POKEMON_NATIONAL(PIDGEOT),
    EC_POKEMON_NATIONAL(PIDGEOTTO),
    EC_WORD_LEFT,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(PICHU),
    EC_POKEMON(PICHU),
    EC_MOVE2(SCRATCH),
    EC_WORD_SURPRISE,
    EC_MOVE2(HYPER_FANG),
    EC_POKEMON_NATIONAL(CLEFAIRY),
    EC_WORD_DROUGHT,
    EC_WORD_HIDDEN,
    EC_WORD_PERSON,
    EC_WORD_AWFUL,
    EC_POKEMON_NATIONAL(CHARMANDER),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(STARYU),
    EC_POKEMON(STARYU),
    EC_WORD_ALONE,
    EC_POKEMON_NATIONAL(CYNDAQUIL),
    EC_MOVE2(EMBER),
    EC_POKEMON(VIBRAVA),
    EC_WORD_BORED,
    EC_POKEMON_NATIONAL(SUNKERN),
    EC_WORD_SECRET,
    EC_WORD_SECRET_BASE,
    EC_MOVE2(SECRET_POWER),
    EC_POKEMON_NATIONAL(TEDDIURSA),
    EC_WORD_HIYAH,
    EC_MOVE2(DIZZY_PUNCH),
    EC_WORD_LIGHTNINGROD,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(VOLTORB),
    EC_POKEMON(VOLTORB),
    EC_WORD_DAYTIME,
    EC_MOVE2(BULK_UP),
    EC_WORD_NAP,
    EC_WORD_HEROINE,
    EC_POKEMON(FEEBAS),
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseFu[] = {
    EC_WORD_FIGHT,
    EC_POKEMON_NATIONAL(MOLTRES),
    EC_WORD_FASHION,
    EC_WORD_FEAR,
    EC_WORD_FEVER,
    EC_MOVE2(IMPRISON),
    EC_POKEMON_NATIONAL(FLAREON),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(ALAKAZAM),
    EC_POKEMON(ALAKAZAM),
    EC_POKEMON_NATIONAL(MAGMAR),
    EC_POKEMON(GRUMPIG),
    EC_MOVE2(FEATHER_DANCE),
    EC_POKEMON_NATIONAL(FORRETRESS),
    EC_MOVE2(WHIRLWIND),
    EC_WORD_COMPOUNDEYES,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(WIGGLYTUFF),
    EC_POKEMON(WIGGLYTUFF),
    EC_MOVE2(BEAT_UP),
    EC_WORD_MYSTERY,
    EC_POKEMON_NATIONAL(IVYSAUR),
    EC_POKEMON_NATIONAL(BULBASAUR),
    EC_WORD_MARVEL_SCALE,
    EC_WORD_WONDER_GUARD,
    EC_POKEMON_NATIONAL(VENUSAUR),
    EC_WORD_LACKS,
    EC_WORD_RESUSCITATE,
    EC_WORD_SMACK,
    EC_WORD_FUFUFU,
    EC_POKEMON_NATIONAL(AERODACTYL),
    EC_WORD_WIMPY,
    EC_POKEMON_NATIONAL(MAGBY),
    EC_WORD_LOL,
    EC_MOVE2(BLIZZARD),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(IGGLYBUFF),
    EC_POKEMON(IGGLYBUFF),
    EC_WORD_SNORT,
    EC_MOVE2(STOMP),
    EC_WORD_INSOMNIA,
    EC_WORD_WINTER,
    EC_WORD_LEVITATE,
    EC_POKEMON(FLYGON),
    EC_WORD_PLUS,
    EC_MOVE2(BLAST_BURN),
    EC_POKEMON(PLUSLE),
    EC_POKEMON_NATIONAL(UMBREON),
    EC_MOVE2(FLASH),
    EC_WORD_WOBBLY,
    EC_MOVE2(TEETER_DANCE),
    EC_WORD_PRETEND,
    EC_POKEMON_NATIONAL(ARTICUNO),
    EC_WORD_WANNABE,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(JIGGLYPUFF),
    EC_POKEMON(JIGGLYPUFF),
    EC_POKEMON_NATIONAL(SNUBBULL),
    EC_WORD_SHAKY,
    EC_MOVE2(CRUSH_CLAW),
    EC_MOVE2(BLAZE_KICK),
    EC_MOVE2(PRESENT),
    EC_WORD_PRESSURE,
    EC_WORD_BLEND,
    EC_WORD_FRONTIER,
    EC_WORD_HUMPH,
    EC_MOVE2(ERUPTION),
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseHe[] = {
    EC_WORD_HEY,
    EC_POKEMON(CORPHISH),
    EC_WORD_BABY,
    EC_POKEMON_NATIONAL(BAYLEEF),
    EC_WORD_BEST,
    EC_WORD_LOUSY,
    EC_WORD_HEHEHE,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(GRIMER),
    EC_POKEMON(GRIMER),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(MUK),
    EC_POKEMON(MUK),
    EC_WORD_LIQUID_OOZE,
    EC_MOVE2(SLUDGE),
    EC_MOVE2(SLUDGE_BOMB),
    EC_MOVE2(GLARE),
    EC_WORD_HEH,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(HERACROSS),
    EC_POKEMON(HERACROSS),
    EC_POKEMON(PELIPPER),
    EC_POKEMON_NATIONAL(HOUNDOOM),
    EC_POKEMON_NATIONAL(PERSIAN),
    EC_WORD_RIPPED,
    EC_POKEMON_NATIONAL(LICKITUNG),
    EC_WORD_STUDY,
    EC_WORD_COLOR_CHANGE,
    EC_MOVE2(TRANSFORM),
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseHo[] = {
    EC_MOVE2(POISON_TAIL),
    EC_WORD_POINTS,
    EC_POKEMON_NATIONAL(HO_OH),
    EC_WORD_SOUNDPROOF,
    EC_WORD_ADVENTURE,
    EC_WORD_EFFECT_SPORE,
    EC_MOVE2(ROAR),
    EC_POKEMON(WAILORD),
    EC_POKEMON(WAILMER),
    EC_WORD_DUDE,
    EC_WORD_BOARD,
    EC_POKEMON_NATIONAL(HOOTHOOT),
    EC_POKEMON(SALAMENCE),
    EC_WORD_BALL,
    EC_MOVE2(BONE_RUSH),
    EC_WORD_THEY,
    EC_WORD_THEY_WERE,
    EC_WORD_TO_THEM,
    EC_WORD_THEIR,
    EC_WORD_THEY_RE,
    EC_WORD_THEY_VE,
    EC_WORD_POKENAV,
    EC_WORD_POKEMON,
    EC_WORD_GET,
    EC_WORD_POKEDEX,
    EC_WORD_PKRS,
    EC_MOVE2(CAMOUFLAGE),
    EC_WORD_WANT,
    EC_MOVE2(COVET),
    EC_POKEMON(AGGRON),
    EC_MOVE2(TAIL_GLOW),
    EC_POKEMON(POOCHYENA),
    EC_POKEMON_NATIONAL(PIDGEY),
    EC_WORD_AS_MUCH_AS,
    EC_POKEMON_NATIONAL(PONYTA),
    EC_MOVE2(BONE_CLUB),
    EC_MOVE2(BONEMERANG),
    EC_WORD_FIRE,
    EC_MOVE2(FIRE_SPIN),
    EC_WORD_FLAME_BODY,
    EC_MOVE2(FIRE_PUNCH),
    EC_POKEMON_NATIONAL(SKIPLOOM),
    EC_WORD_HOHOHO,
    EC_WORD_PRAISE,
    EC_WORD_UH_HUH,
    EC_POKEMON_NATIONAL(PORYGON),
    EC_POKEMON_NATIONAL(PORYGON2),
    EC_MOVE2(VOLT_TACKLE),
    EC_WORD_POKEBLOCK,
    EC_MOVE2(PERISH_SONG),
    EC_WORD_SHREDDED,
    EC_POKEMON(CASTFORM),
    EC_WORD_BOOK,
    EC_WORD_SERIOUS,
    EC_WORD_REALLY,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseMa[] = {
    EC_WORD_OH_DEAR,
    EC_WORD_IN,
    EC_WORD_GIVE_UP,
    EC_WORD_MINUS,
    EC_POKEMON(MINUN),
    EC_WORD_DAILY,
    EC_WORD_OWN_TEMPO,
    EC_MOVE2(WRAP),
    EC_MOVE2(SPIKES),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(MAGCARGO),
    EC_POKEMON(MAGCARGO),
    EC_MOVE2(MAGNITUDE),
    EC_POKEMON(MAKUHITA),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(SLUGMA),
    EC_POKEMON(SLUGMA),
    EC_WORD_MAGMA_ARMOR,
    EC_POKEMON_NATIONAL(QUILAVA),
    EC_WORD_OVERDO,
    EC_WORD_LOSS,
    EC_WORD_IF_I_LOSE,
    EC_WORD_LOST,
    EC_WORD_LOSE,
    EC_WORD_TRULY,
    EC_WORD_SERIOUSLY,
    EC_MOVE2(MAGICAL_LEAF),
    EC_MOVE2(MAGIC_COAT),
    EC_WORD_MACHINE,
    EC_WORD_OUT,
    EC_WORD_MASTER,
    EC_WORD_INCREASING,
    EC_WORD_TOO,
    EC_WORD_LIKE,
    EC_POKEMON_NATIONAL(BELLSPROUT),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(WEEZING),
    EC_POKEMON(WEEZING),
    EC_WORD_SMELL,
    EC_WORD_YET,
    EC_WORD_MISTAKE,
    EC_POKEMON(LINOONE),
    EC_WORD_TOTALLY,
    EC_MOVE2(MUD_SHOT),
    EC_MOVE2(MACH_PUNCH),
    EC_WORD_FESTIVAL,
    EC_WORD_UNTIL,
    EC_WORD_WAIT,
    EC_WORD_GUARD,
    EC_MOVE2(PROTECT),
    EC_POKEMON(CASCOON),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(MARILL),
    EC_POKEMON(MARILL),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(AZUMARILL),
    EC_POKEMON(AZUMARILL),
    EC_MOVE2(DEFENSE_CURL),
    EC_WORD_AS_IF,
    EC_POKEMON(SWALOT),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(ELECTRODE),
    EC_POKEMON(ELECTRODE),
    EC_MOVE2(ROLLING_KICK),
    EC_WORD_COMICS,
    EC_POKEMON_NATIONAL(MANKEY),
    EC_WORD_SATISFIED,
    EC_POKEMON_NATIONAL(MANTINE),
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseMi[] = {
    EC_WORD_PARTNER,
    EC_MOVE2(SUBSTITUTE),
    EC_WORD_RIGHT,
    EC_MOVE2(DETECT),
    EC_MOVE2(PIN_MISSILE),
    EC_WORD_WATER,
    EC_MOVE2(WATER_SPORT),
    EC_POKEMON(MUDKIP),
    EC_MOVE2(WATER_GUN),
    EC_MOVE2(MIST_BALL),
    EC_MOVE2(WATER_PULSE),
    EC_WORD_WATER_VEIL,
    EC_WORD_SHOW,
    EC_WORD_SEE,
    EC_WORD_DID,
    EC_MOVE2(FURY_ATTACK),
    EC_MOVE2(FURY_SWIPES),
    EC_MOVE2(DESTINY_BOND),
    EC_WORD_LOOKS,
    EC_WORD_REJECT,
    EC_WORD_ACCEPT,
    EC_POKEMON_NATIONAL(DRATINI),
    EC_MOVE2(FALSE_SWIPE),
    EC_MOVE2(FORESIGHT),
    EC_POKEMON_NATIONAL(MEW),
    EC_POKEMON_NATIONAL(MEWTWO),
    EC_MOVE2(MIRROR_COAT),
    EC_MOVE2(FUTURE_SIGHT),
    EC_WORD_SEES,
    EC_MOVE2(MILK_DRINK),
    EC_POKEMON_NATIONAL(MILTANK),
    EC_POKEMON(MILOTIC),
    EC_WORD_WE,
    EC_WORD_BEEN,
    EC_WORD_TO_US,
    EC_WORD_OUR,
    EC_WORD_WE_RE,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseMu[] = {
    EC_WORD_MOOD,
    EC_POKEMON_NATIONAL(MISDREAVUS),
    EC_WORD_OLDEN,
    EC_WORD_ARRGH,
    EC_WORD_BUG,
    EC_WORD_SWARM,
    EC_WORD_RATHER,
    EC_POKEMON_NATIONAL(SMOOCHUM),
    EC_WORD_INVINCIBLE,
    EC_WORD_HEART,
    EC_WORD_MUFUFU,
    EC_WORD_MMM,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseMe[] = {
    EC_MOVE2(CALM_MIND),
    EC_WORD_MAIL,
    EC_MOVE2(MEGA_DRAIN),
    EC_MOVE2(MEGA_KICK),
    EC_MOVE2(MEGA_PUNCH),
    EC_POKEMON_NATIONAL(MEGANIUM),
    EC_MOVE2(MEGAHORN),
    EC_WORD_SEEK,
    EC_MOVE2(HIDDEN_POWER),
    EC_WORD_RARE,
    EC_POKEMON(METAGROSS),
    EC_POKEMON_NATIONAL(DITTO),
    EC_MOVE2(METAL_CLAW),
    EC_POKEMON(METANG),
    EC_WORD_DESTROYED,
    EC_WORD_MESSAGE,
    EC_WORD_AWFULLY,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(TENTACOOL),
    EC_POKEMON(TENTACOOL),
    EC_WORD_FIERY,
    EC_POKEMON_NATIONAL(MAREEP),
    EC_MOVE2(ATTRACT),
    EC_WORD_CUTE_CHARM,
    EC_WORD_IMMUNITY,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseMo[] = {
    EC_WORD_ALMOST,
    EC_WORD_BLAZE,
    EC_WORD_NEARLY,
    EC_WORD_MODE,
    EC_WORD_OLD,
    EC_WORD_THURSDAY,
    EC_POKEMON_NATIONAL(FLAAFFY),
    EC_WORD_LISTENING,
    EC_WORD_OWN,
    EC_WORD_MORE,
    EC_WORD_DIDN_T,
    EC_WORD_PICKUP,
    EC_MOVE2(MIMIC),
    EC_WORD_TOO_LATE,
    EC_WORD_PATTERN,
    EC_WORD_FLASH_FIRE,
    EC_WORD_RECEIVED,
    EC_POKEMON_NATIONAL(VENOMOTH),
    EC_WORD_DOESN_T,
    EC_POKEMON_NATIONAL(TANGELA),
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseYa[] = {
    EC_WORD_HOO_HAH,
    EC_WORD_PROMISE,
    EC_WORD_KIND,
    EC_POKEMON(BALTOY),
    EC_WORD_HOLIDAY,
    EC_WORD_NEGATIVE,
    EC_MOVE2(FRUSTRATION),
    EC_WORD_DONE,
    EC_WORD_FINALLY,
    EC_WORD_ANY,
    EC_WORD_YAHOO,
    EC_POKEMON_NATIONAL(SLOWKING),
    EC_POKEMON_NATIONAL(SLOWBRO),
    EC_MOVE2(LEECH_SEED),
    EC_POKEMON_NATIONAL(SLOWPOKE),
    EC_WORD_DANGER,
    EC_POKEMON_NATIONAL(MURKROW),
    EC_POKEMON(SABLEYE),
    EC_WORD_DEFEATED,
    EC_WORD_BEAT,
    EC_WORD_VITAL_SPIRIT,
    EC_POKEMON(VIGOROTH),
    EC_WORD_YOUNG,
    EC_POKEMON_NATIONAL(YANMA),
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseYu[] = {
    EC_POKEMON(SNORUNT),
    EC_WORD_TAKE,
    EC_WORD_UNION,
    EC_MOVE2(METRONOME),
    EC_WORD_DREAM,
    EC_MOVE2(DREAM_EATER),
    EC_WORD_ALLOW,
    EC_WORD_FORGET,
    EC_POKEMON(CRADILY),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(KADABRA),
    EC_POKEMON(KADABRA),
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseYo[] = {
    EC_WORD_WITHOUT,
    EC_WORD_YO,
    EC_MOVE2(ACID),
    EC_WORD_COME_OVER,
    EC_WORD_KINDERGARTEN,
    EC_WORD_CHLOROPHYLL,
    EC_WORD_AFTER,
    EC_POKEMON_NATIONAL(LARVITAR),
    EC_WORD_OH_KAY,
    EC_WORD_BEFORE,
    EC_WORD_GREAT,
    EC_MOVE2(MEDITATE),
    EC_WORD_PURE_POWER,
    EC_WORD_WELL,
    EC_MOVE2(SNATCH),
    EC_WORD_OKAY,
    EC_WORD_PLANS,
    EC_WORD_NIGHTTIME,
    EC_WORD_WHILE,
    EC_POKEMON(DUSKULL),
    EC_WORD_EASY,
    EC_WORD_INSTEAD,
    EC_WORD_NIGHT,
    EC_POKEMON_NATIONAL(NOCTOWL),
    EC_WORD_COUNT_ON,
    EC_WORD_WEAK,
    EC_WORD_TOO_WEAK,
    EC_WORD_WEAKENED,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseRa[] = {
    EC_POKEMON_NATIONAL(RAIKOU),
    EC_WORD_WEEK,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(RAICHU),
    EC_POKEMON(RAICHU),
    EC_WORD_RIVAL,
    EC_WORD_LIFE,
    EC_POKEMON(MANECTRIC),
    EC_WORD_SIMPLE,
    EC_WORD_PUSHOVER,
    EC_POKEMON(SWAMPERT),
    EC_POKEMON(ELECTRIKE),
    EC_WORD_SEEMS,
    EC_WORD_RADIO,
    EC_MOVE2(LUSTER_PURGE),
    EC_POKEMON_NATIONAL(CHANSEY),
    EC_POKEMON_NATIONAL(RATICATE),
    EC_POKEMON(LATIAS),
    EC_POKEMON(LATIOS),
    EC_POKEMON(LUVDISC),
    EC_POKEMON_NATIONAL(LAPRAS),
    EC_WORD_LOVEY_DOVEY,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(VILEPLUME),
    EC_POKEMON(VILEPLUME),
    EC_WORD_LALALA,
    EC_POKEMON(RALTS),
    EC_WORD_FORGETS,
    EC_WORD_APPEARS,
    EC_WORD_RANK,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(LANTURN),
    EC_POKEMON(LANTURN),
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseRi[] = {
    EC_WORD_LEADER,
    EC_MOVE2(LEAF_BLADE),
    EC_POKEMON_NATIONAL(CHARMELEON),
    EC_POKEMON_NATIONAL(CHARIZARD),
    EC_MOVE2(RECYCLE),
    EC_MOVE2(REFLECT),
    EC_MOVE2(REFRESH),
    EC_MOVE2(REVENGE),
    EC_WORD_RIBBON,
    EC_WORD_TRENDY,
    EC_MOVE2(DRAGON_RAGE),
    EC_MOVE2(DRAGON_BREATH),
    EC_MOVE2(DRAGON_DANCE),
    EC_WORD_VACATION,
    EC_POKEMON(LILEEP),
    EC_POKEMON_NATIONAL(URSARING),
    EC_WORD_SHIELD_DUST,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseRu[] = {
    EC_POKEMON_NATIONAL(JYNX),
    EC_WORD_ROOM,
    EC_WORD_RULE,
    EC_POKEMON_NATIONAL(LUGIA),
    EC_WORD_LOOK,
    EC_POKEMON(LUNATONE),
    EC_WORD_RUBY,
    EC_WORD_THAN,
    EC_POKEMON(AZURILL),
    EC_POKEMON(LUDICOLO),
    EC_WORD_HAPPILY,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseRe[] = {
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(MAGNETON),
    EC_POKEMON(MAGNETON),
    EC_MOVE2(ICE_PUNCH),
    EC_MOVE2(ICE_BEAM),
    EC_POKEMON(REGICE),
    EC_POKEMON(REGISTEEL),
    EC_POKEMON(REGIROCK),
    EC_POKEMON(RAYQUAZA),
    EC_WORD_UGLY,
    EC_POKEMON_NATIONAL(LEDIAN),
    EC_POKEMON_NATIONAL(LEDYBA),
    EC_WORD_FANTASTIC,
    EC_WORD_LEVEL,
    EC_MOVE2(FURY_CUTTER),
    EC_MOVE2(COMET_PUNCH),
    EC_WORD_RENTAL,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseRo[] = {
    EC_MOVE2(SKULL_BASH),
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(VULPIX),
    EC_POKEMON(VULPIX),
    EC_POKEMON(ROSELIA),
    EC_MOVE2(LOCK_ON),
    EC_MOVE2(ROCK_BLAST),
    EC_WORD_ROMANTIC,
    EC_WORD_QUESTION,
};

const u16 gEasyChatWordsByLetter_UnusedJapaneseWa[] = {
    EC_WORD_YAY,
    EC_WORD_WORLD,
    EC_WORD_AWW,
    EC_WORD_WIRELESS,
    EC_WORD_WOWEE,
    EC_POKEMON(COMBUSKEN),
    EC_WORD_UNDERSTAND,
    EC_WORD_UNDERSTANDS,
    EC_WORD_FAINT,
    EC_WORD_ANTICIPATION,
    EC_WORD_ONCE,
    EC_WORD_MOVE,
    EC_WORD_WE_VE,
    EC_WORD_FAINTED,
    EC_WORD_WOMAN,
    EC_WORD_SHE,
    EC_WORD_SHE_WAS,
    EC_WORD_TO_HER,
    EC_WORD_HERS,
    EC_WORD_SHE_IS,
    EC_WORD_SOME,
    EC_POKEMON_NATIONAL(JUMPLUFF),
    EC_MOVE2(COTTON_SPORE),
    EC_WORD_GWAH,
    EC_POKEMON_NATIONAL(TOTODILE),
    EC_WORD_WAHAHAHA,
    EC_WORD_ANYWHERE,
    EC_MOVE2(STRUGGLE),
    EC_WORD_BADLY,
    DOUBLE_SPECIES_NAME
    EC_POKEMON_NATIONAL(MACHOP),
    EC_POKEMON(MACHOP),
};

const struct EasyChatWordsByLetter gEasyChatWordsByLetterPointers[EC_NUM_ALPHABET_GROUPS + 18] = { // 18 unused JP groups
    {
        .words = gEasyChatWordsByLetter_KoreanOthers,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanOthers),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanGiyeok,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanGiyeok),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanSsangGiyeok,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanSsangGiyeok),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanNieun,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanNieun),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanDigeut,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanDigeut),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanSsangDigeut,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanSsangDigeut),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanRieul,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanRieul),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanMieum,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanMieum),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanBieup,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanBieup),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanSsangBieup,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanSsangBieup),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanSiot,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanSiot),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanSsangSiot,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanSsangSiot),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanIeung,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanIeung),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanJieut,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanJieut),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanSsangJieut,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanSsangJieut),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanChieut,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanChieut),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanKieuk,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanKieuk),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanTieut,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanTieut),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanPieup,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanPieup),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanHieut,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_KoreanHieut),
    },
    {
        .words = gEasyChatWordsByLetter_KoreanUnused20,
        .numWords = 0,
    },
    {
        .words = gEasyChatWordsByLetter_KoreanUnused21,
        .numWords = 0,
    },
    {
        .words = gEasyChatWordsByLetter_KoreanUnused22,
        .numWords = 0,
    },
    {
        .words = gEasyChatWordsByLetter_KoreanUnused23,
        .numWords = 0,
    },
    {
        .words = gEasyChatWordsByLetter_KoreanUnused24,
        .numWords = 0,
    },
    {
        .words = gEasyChatWordsByLetter_KoreanUnused25,
        .numWords = 0,
    },
    {
        .words = gEasyChatWordsByLetter_KoreanUnused26,
        .numWords = 0,
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseHi,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseHi),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseFu,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseFu),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseHe,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseHe),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseHo,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseHo),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseMa,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseMa),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseMi,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseMi),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseMu,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseMu),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseMe,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseMe),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseMo,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseMo),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseYa,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseYa),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseYu,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseYu),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseYo,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseYo),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseRa,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseRa),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseRi,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseRi),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseRu,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseRu),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseRe,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseRe),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseRo,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseRo),
    },
    {
        .words = gEasyChatWordsByLetter_UnusedJapaneseWa,
        .numWords = ARRAY_COUNT(gEasyChatWordsByLetter_UnusedJapaneseWa),
    },
};
