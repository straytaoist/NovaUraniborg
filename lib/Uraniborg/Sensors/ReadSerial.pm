package Uraniborg::Sensors::ReadSerial;

use lib '/Users/straytaoist/perl5/lib/perl5/';

use Moose;
use Device::SerialPort::Arduino;

use common::sense;

has reader => (
  is         => 'ro',
  isa        => 'Device::SerialPort::Arduino',
  lazy_build => 1,
  handles    => [ qw/receive/ ],
  );

has port => (
  is      => 'ro',
  isa     => 'Str',
  default => '/dev/tty.usbmodem1411',
  );

has baudrate => (
  is      => 'ro',
  isa     => 'Num',
  default => 9600,
  );


sub _build_reader {
  my $self = shift;
  return Device::SerialPort::Arduino->new(
    port     => $self->port,
    baudrate => $self->baudrate,
    );
  }

sub BUILD {
  my $self = shift;
  # Ignore first junk reading, who knows when we hop on to the serial port?
  $self->receive;
  }

sub classify_input {
  my $input = shift->receive;
  my $message = '';
  if ($input =~ m/degrees C/) {
    $message = "Current location: office\nCurrent temperature: $input";
    }
  else {
    die "Unknown input: $input";
    }
  return $message;
  }

__PACKAGE__->meta->make_immutable;
