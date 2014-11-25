package Uraniborg::Twitter;

use Moose;

with 'MooseX::Getopt';

use common::sense;

use Net::Twitter::Lite::WithAPIv1_1;

has connection => (
    is      => 'ro',
    isa     => 'Net::Twitter::Lite::WithAPIv1_1',
    builder => '_build_connection',
    handles => { tweet => 'update' },
    lazy    => 1,
    );

has consumer_key => (
    is       => 'ro',
    isa      => 'Str',
    required => 1,
    );

has consumer_secret => (
    is       => 'ro',
    isa      => 'Str',
    required => 1,
    );

has access_token => (
    is       => 'ro',
    isa      => 'Str',
    required => 1,
    );

has access_token_secret => (
    is       => 'ro',
    isa      => 'Str',
    required => 1,
    );

sub _build_connection {
    my $self = shift;
    return Net::Twitter::Lite::WithAPIv1_1->new(
        consumer_key        => $self->consumer_key,
        consumer_secret     => $self->consumer_secret,
        access_token        => $self->access_token,
        access_token_secret => $self->access_token_secret,
        ssl                 => 1,
        );
    }

__PACKAGE__->meta->make_immutable;
