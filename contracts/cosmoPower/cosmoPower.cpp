#include "cosmoPower.hpp"
#include "../cosmoCoin/cosmoCoin.hpp"

cosmoPower::cosmoPower( account_name self ) :contract(self),_accounts( _self, _self) {
    
}

void cosmoPower::transfer( account_name from, account_name to, uint64_t quantity ) {
    require_auth( from );

    const auto& fromacnt = _accounts.get( from );
    eosio_assert( fromacnt.balance >= quantity, "overdrawn balance" );
    _accounts.modify( fromacnt, from, [&]( auto& a ){ a.balance -= quantity; } );

    add_balance( from, to, quantity );
}

void cosmoPower::issue( account_name to, uint64_t quantity ) {
    require_auth( _self );
    add_balance( _self, to, quantity );
}

void cosmoPower::burn( account_name to, uint64_t quantity ) {
    require_auth( _self );
    sub_balance( _self, to, quantity );
}

void cosmoPower::defer_exchange( account_name power, account_name coin, uint64_t powerQuantity) {
    require_auth( coin );
    
    const auto& poweracnt = _accounts.get( power );
    eosio_assert( poweracnt.balance >= powerQuantity, "overdrawn balance" );
    _accounts.modify( poweracnt, power, [&]( auto& a ){ a.balance -= powerQuantity; } );

    change_balance( power, coin, powerQuantity );
}

auto cosmoPower::getPowerAccount( account_name inputName ) {
    return _accounts.get( inputName );
}

void cosmoPower::add_balance( account_name payer, account_name to, uint64_t q ) {
    auto toitr = _accounts.find( to );
    if( toitr == _accounts.end() ) {
        _accounts.emplace( payer, [&]( auto& a ) {
            a.owner = to;
            a.balance = q;
        });
    } else {
        _accounts.modify( toitr, 0, [&]( auto& a ) {
            a.balance += q;
            eosio_assert( a.balance >= q, "overflow detected" );
        });
    }
}

void cosmoPower::sub_balance( account_name payer, account_name to, uint64_t q ) {
    auto toitr = _accounts.find( to );
    if( toitr == _accounts.end() ) {
        //sub something
    } else {
        _accounts.modify( toitr, 0, [&]( auto& a ) {
            a.balance -= q;
            eosio_assert( a.balance <= q, "overflow detected" );
        });
    }
}

void cosmoPower::change_balance( account_name powerFrom, account_name coinTo, uint64_t q) {
    auto toitr = _accounts.find( coinTo );
    if( toitr == _accounts.end() ) {
        //No power account, call account
    } else {
        _accounts.modify( toitr, 0, [&]( auto& a ) {
        a.balance += q;
        eosio_assert( a.balance >= q, "overflow detected" );
        });
    } 
}

EOSIO_ABI( cosmoPower, (issue)(transfer)(defer_exchange))
