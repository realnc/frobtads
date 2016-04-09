#include <tads.h>

main()
{
    test('this is a subject string', '');
    test('this is a subject string', 'this');
    test('this is a subject string', 'is a subject');
    test('this is a subject string', 'is a subject', 5);
    test('this is a subject string', 'is a subject', 6);
    test('this is a subject string', 'is a subject', 7);
    test('this is a subject string', 'string');
    test('this is a subject string', 'string', -4);
    test('this is a subject string', 'string', -5);
    test('this is a subject string', 'string', -6);
    test('this is a subject string', 'string', -7);

    test('this is a subject string', R'%w*');
    test('this is a subject string', R'%w+');
    test('this is a subject string', R'%w*', 5);
    test('this is a subject string', R'%w+', 5);
    test('this is a subject string', R'%w+', 6);
    test('this is a subject string', R'%w+', -4);
    test('this is a subject string', R'%w+', -5);
    test('this is a subject string', R'%w+', -6);
    test('this is a subject string', R'%w+', -7);
}

test(subj, pat, idx?)
{
    local match = (idx == nil ? subj.match(pat) : subj.match(pat, idx));
    "'<<subj>>'.match(<<if pat.ofKind(RexPattern)>>R<<end>>'<<pat>>'<<
      if idx != nil>>, <<idx>><<end>>) = <<
      if match == nil>>nil<<else>><<match>><<end>>\n";
}


