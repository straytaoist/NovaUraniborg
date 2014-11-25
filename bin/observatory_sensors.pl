#!/usr/bin/perl -w

=head1 NAME

  observatory_sensors.pl - read the observatory sensors

=head1 SYNOPSIS

  perl -w bin/observatory_sensors.pl --consumer_key $consumer_key
                                     --consumer_secret $consumer_secret
                                     --access_token $access_token
                                     --access_token_secret $access_token_secret

=head1 TODO

  o turn this into a daemon

=cut

use lib 'lib';

use Uraniborg::Twitter;
use Uraniborg::Sensors::ReadSerial;

use Time::Seconds;
use Try::Tiny;

my $twitter = Uraniborg::Twitter->new_with_options;
my $reader = Uraniborg::Sensors::ReadSerial->new;

sub _record_temp {
    (my $temp = shift) =~ m/(\d\d\.\d\d)/;
    open OUT, '>>', 'temp.csv';
    print OUT "$1\n";
    close OUT;
}

while (1) {
    my $message = $reader->classify_input;
    _record_temp($message);
    try {
        $twitter->tweet($message);
        print "Tweeted: $message\n";
        }
    catch {
        print "$_\n";
        };
    sleep ONE_HOUR;
    }
