#include <stdlib.h>
#include <check.h>
#include <math.h>
#include "../src/rotator.h"

rotator *rot = NULL;

void assertRotate(float in1, float in2, float out1, float out2) {
	float complex input = in1 + in2 * I;
	float complex actual = rotator_increment(rot, input);
	ck_assert_float_eq_tol(out1, crealf(actual), 0.00001f);
	ck_assert_float_eq_tol(out2, cimagf(actual), 0.00001f);
}

START_TEST (test_success) {
	float complex phase = 1 + 0 * I;
	float complex phase_incr = cosf(2 * M_PI / 1003) + sinf(2 * M_PI / 1003) * I;
	int code = create_rotator(phase, phase_incr, &rot);
	ck_assert_int_eq(code, 0);
	assertRotate(1.0f, 0.0f, 1.0f, 0.0f);
	assertRotate(1.0f, 0.0f, 0.99998f, 0.006264f);
	assertRotate(1.0f, 0.0f, 0.999922f, 0.012528f);
	assertRotate(1.0f, 0.0f, 0.999823f, 0.018792f);
}
END_TEST

void teardown() {
	destroy_rotator(rot);
}

void setup() {
	//do nothing
}

Suite* common_suite(void) {
	Suite *s;
	TCase *tc_core;

	s = suite_create("rotator");

	/* Core test case */
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_success);

	tcase_add_checked_fixture(tc_core, setup, teardown);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void) {
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = common_suite();
	sr = srunner_create(s);

	srunner_set_fork_status(sr, CK_NOFORK);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

