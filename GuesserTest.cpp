/**
 * Unit Tests for the class
**/

#include <gtest/gtest.h>
#include "Guesser.h"

using namespace std;

class GuesserTest : public ::testing::Test
{
	protected:
		GuesserTest(){} //constructor runs before each test
		virtual ~GuesserTest(){} //destructor cleans up after tests
		virtual void SetUp() {} //sets up before each test (after constructor)
		virtual void TearDown() {} //clean up after each test, (before destructor)
};

// Result: PASS
// Test cases for the Guesser class
TEST(GuesserTest, Init) {
    Guesser guessing("myPass");
    ASSERT_EQ(guessing.remaining(), 3);
}

// Result: PASS 
// Test cases for the match function and still remaining guesses are still 3 available.
TEST(GuesserTest, Match) {
    Guesser guessing("myPass");
    ASSERT_TRUE(guessing.match("myPass"));
}

// Result: PASS
// Test cases for the match function and remaining guesses are decremented.
TEST(GuesserTest, FirstWrongGuess) {
    Guesser guessing("myPass");
    ASSERT_FALSE(guessing.match("myPas$"));
    ASSERT_EQ(guessing.remaining(), 2);
}

// Result: PASS
// Test cases for the match function and remaining guesses are decremented.
TEST(GuesserTest, NoMatch) {
    Guesser guessing("annnn");
    ASSERT_FALSE(guessing.match("qqqq"));
}

// Result: PASS
// After 3 wrong guesses, secret is locked out.
TEST(GuesserTest, NoGuessesAllowedAfterLock) {
    Guesser guessing("myPass");
    guessing.match("myPazs");
    guessing.match("myPa$s");
    guessing.match("myPas0");
    ASSERT_FALSE(guessing.match("myPass"));
    ASSERT_EQ(guessing.remaining(), 0);
}

// Result: PASS
// longer than 32 is truncated but still allows matching prefix.
TEST(GuesserTest, LongerThan32) {
    Guesser guessing("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
    ASSERT_TRUE(guessing.match("ABCDEFGHIJKLMNOPQRSTUVWXYZ123456"));
}

// Result: PASS
// <32 chars are not truncated.
TEST(GuesserTest, DoesNotTruncateSecretShorterThan32) {
    Guesser guessing("shortPass");
    ASSERT_TRUE(guessing.match("shortPass"));
}

// Result: PASS
// Empty secret can be matched.
TEST(GuesserTest, HandlesEmptySecret) {
    Guesser guessing("");
    ASSERT_TRUE(guessing.match(""));
    ASSERT_EQ(guessing.remaining(), 3);
}

// Result: PASS
// Matching is case-sensitive.
TEST(GuesserTest, MatchIsCaseSensitive) {
    Guesser guessing("Abd");
    ASSERT_FALSE(guessing.match("abd"));
}

// Result: PASS
// Matching with special characters.
TEST(GuesserTest, MatchWithSpecialCharacters) {
    Guesser guessing("P@ssw0rd!");
    ASSERT_TRUE(guessing.match("P@ssw0rd!"));
    ASSERT_FALSE(guessing.match("p@ssw0rd!"));
}

// Result: PASS
// Short secrets match exactly, longer guesses don’t match.
TEST(GuesserTest, AcceptsShorterSecret) {
    Guesser guessing("abc");
    ASSERT_TRUE(guessing.match("abc"));
    ASSERT_FALSE(guessing.match("abcd"));
}

// Result: PASS
//  Guess with large edit distance locks.
TEST(GuesserTest, DistanceGreaterThanTwoLocksSecret) {
    Guesser guessing("abcd");
    ASSERT_FALSE(guessing.match("what"));
    ASSERT_FALSE(guessing.match("abcd"));
}

// Result: PASS
// Single character secret will match case exactly only.
TEST(GuesserTest, SingleCharacterSecret) {
    Guesser guessing("A");
    ASSERT_TRUE(guessing.match("A"));
    ASSERT_FALSE(guessing.match("a"));
}

// Result: PASS
// Check for case sensitivity test.
TEST(GuesserTest, CaseSensitivity) {
    Guesser guessing("Secret");
    ASSERT_FALSE(guessing.match("secret"));
    ASSERT_TRUE(guessing.match("Secret"));
}

// Result: PASS
// No more guess after lock (3 failed attempts).
TEST(GuesserTest, NoGuessAfterLock) {
    Guesser guessing("Secret");
    guessing.match("Secrex");
    guessing.match("Secrey");
    guessing.match("Secrez");
    ASSERT_FALSE(guessing.match("Secret"));
}

// Result: PASS
// Test to check if Non-alphanumeric characters are matched properly/
TEST(GuesserTest, NonAlphanumericChars) {
    Guesser guessing("Secre$T123");
    ASSERT_TRUE(guessing.match("Secre$T123"));
    ASSERT_FALSE(guessing.match("Secret123"));
}

// Result: PASS
// Test to check if special characters are matched properly.
TEST(GuesserTest, SpecialCharacter) {
    Guesser guessing("P@ssw0rd!");
    ASSERT_TRUE(guessing.match("P@ssw0rd!"));
    ASSERT_FALSE(guessing.match("p@ssw0rd!"));
    ASSERT_FALSE(guessing.match("P@ssw0rd"));
}

// Result: PASS
// Test to check if many special characters are matched properly.
TEST(GuesserTest, ManySpecialCharacters) {
    Guesser guessing("P@ssw0rd!#");
    ASSERT_TRUE(guessing.match("P@ssw0rd!#"));
    ASSERT_FALSE(guessing.match("P@ssw0rd!"));
    ASSERT_FALSE(guessing.match("P@ssw0rd#"));
}

// Result: PASS
// Test to check if long password with special characters is matched properly.
TEST(GuesserTest, LongPassWithSpecialChars) {
    Guesser guessing("A!B@C#D$E%F^G&H*I(J)K");
    ASSERT_TRUE(guessing.match("A!B@C#D$E%F^G&H*I(J)K"));
    ASSERT_FALSE(guessing.match("A!B@C#D$E%F^G&H*I(J)K!"));
    ASSERT_FALSE(guessing.match("A!B@C#D$E%F^G&H*I(J)K#"));
}

// Result: PASS
// Test to check if  spaces is matched properly.
TEST(GuesserTest, WithSpaces) {
    Guesser guessing("Hi World Hello");
    ASSERT_TRUE(guessing.match("Hi World Hello"));
    ASSERT_FALSE(guessing.match("HiWorldHellos"));
    ASSERT_FALSE(guessing.match("Hi WorldHello"));
    ASSERT_FALSE(guessing.match("Hi World Hello!"));
}

// Result: PASS
// Test to check if  mixed case is matched properly.
TEST(GuesserTest, MixedCase) {
    Guesser guessing("MiXeD CaSe");
    ASSERT_TRUE(guessing.match("MiXeD CaSe"));
    ASSERT_FALSE(guessing.match("mixed case"));
    ASSERT_FALSE(guessing.match("MIXED CASE"));
    ASSERT_FALSE(guessing.match("Mixed Case!"));
}

// Result: PASS
// Test to check if  unicode characters is matched properly.
TEST(GuesserTest, CaseWithUnicodeCharacters) {
    Guesser guessing("P@ssw0rd🍀");
    ASSERT_TRUE(guessing.match("P@ssw0rd🍀"));
    ASSERT_FALSE(guessing.match("P@ssw0rd"));
    ASSERT_FALSE(guessing.match("p@ssw0rd🍀"));
}

// Result: PASS
// Test to check if truncation does not match original input.
TEST(GuesserTest, TruncateDoesNotMatchOriginalInput) {
    string Input = "LoremIpsumDolorSitAmet_ExceedsThirtyTwoCharacters!!";
    Guesser guessing(Input);                 
    ASSERT_FALSE(guessing.match(Input));    
}

// Result: PASS
// Emoji must be present to match.
TEST(GuesserTest, WithEmoji) {
    Guesser guessing("Hello🌍");
    ASSERT_TRUE(guessing.match("Hello🌍"));
    ASSERT_FALSE(guessing.match("Hello"));
    ASSERT_FALSE(guessing.match("hello🌍"));
}

// Result: PASS
// Test to check if mixed alphanumeric and special characters is matched properly.
TEST(GuesserTest, MixedAlphanumericAndSpecialChars) {
    Guesser guessing("P@ssw0rd123!");
    ASSERT_TRUE(guessing.match("P@ssw0rd123!"));
    ASSERT_FALSE(guessing.match("P@ssw0rd123"));
    ASSERT_FALSE(guessing.match("p@ssw0rd123!"));
    ASSERT_FALSE(guessing.match("P@ssw0rd123!#"));
}

// Result: PASS
// Correct guesses don't use up tries.
TEST(GuesserTest, CorrectGuessesDontDecrementRemaining) {
    Guesser guessing("alphaBeta");
    guessing.match("alphaBeta");
    guessing.match("alphaBeta");
    guessing.match("alphaBeta");
    ASSERT_EQ(guessing.remaining(), 3);
}

// Result: PASS
// Final guess succeeds if it's correct.
TEST(GuesserTest, CorrectGuessSucceedsOnLastAttempt) {
    Guesser guessing("code123");
    guessing.match("code12X");
    guessing.match("code12Y");
    ASSERT_TRUE(guessing.match("code123"));
}

// Result: FAIL
// Expected: remaining resets to 3 after a correct guess. Actual: remained at 1.
TEST(GuesserTest, CorrectGuessResetsRemainingToThree) {
    Guesser guessing("reSet99");
    guessing.match("reSet98");  
    guessing.match("reSet97");  
    ASSERT_TRUE(guessing.match("reSet99"));      
    ASSERT_EQ(guessing.remaining(), 3);
}

// Result: FAIL
// Expected: remaining becomes 2 after a far-off guess. Actual: still at 3.
TEST(GuesserTest, FarGuessLocksAndConsumesOneTry) {
    Guesser guessing("short");
    ASSERT_FALSE(guessing.match("This guess is way off the mark")); 
    ASSERT_EQ(guessing.remaining(), 2);                            
    ASSERT_FALSE(guessing.match("short"));                         
}

// Result: FAIL
// Expected: all far guesses consume tries (remaining = 0). Actual: remaining stayed at 3.
TEST(GuesserTest, FarGuessStillCountsDownRemaining) {
    Guesser guessing("lockMe");
    guessing.match("totally unrelated guess");   
    guessing.match("another wrong long guess");  
    guessing.match("one more wildly off");       
    ASSERT_EQ(guessing.remaining(), 0);
}

// Result: PASS
// Guess with distance of 2 doesn’t lock.
TEST(GuesserTest, DistanceOfTwoDoesNotLock) {
    Guesser guessing("ABCD");
    ASSERT_FALSE(guessing.match("ABEF"));        
    ASSERT_TRUE (guessing.match("ABCD"));        
}

// Result: PASS
// Guessing after 3 wrong attempts still fails.
TEST(GuesserTest, TryPastRemainingStillFail) {
    Guesser guessing("pastDue");
    guessing.match("pastDuX");                   
    guessing.match("pastDuY");                   
    guessing.match("pastDuZ");                  
    guessing.match("random");                    
    ASSERT_FALSE(guessing.match("pastDue"));    
}

// Result: FAIL
// Expected: far guess locks and remaining decrements. Actual: remaining stayed at 3.
TEST(GuesserTest, FarLockThenCloseGuessStillDecrements) {
    Guesser guessing("pa$$");
    ASSERT_FALSE(guessing.match("this guess is way off base")); 
    ASSERT_EQ (guessing.remaining(), 2);                     
    ASSERT_FALSE(guessing.match("pa$"));  
    ASSERT_EQ(guessing.remaining(), 1);
}


// Result: FAIL
// Expected: remaining becomes 0 after far guess + repeats. Actual: stayed at 3.
TEST(GuesserTest, MoreGuessAfterFarLockReachZero) {
    Guesser guessing("pa$$");
    guessing.match("nothing alike");   
    guessing.match("pa$$");            
    guessing.match("pa$$");            
    ASSERT_EQ(guessing.remaining(), 0);
}