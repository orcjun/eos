#include "cosmoCoin.hpp"
#include "../cosmoPower/cosmoPower.hpp"

cosmoCoin::cosmoCoin( account_name self ):contract(self),_accounts( _self, _self) {
    //make cosmoPower??
    //make something....
}

void cosmoCoin::transfer( account_name from, account_name to, uint64_t quantity ) {
    require_auth( from );

    const auto& fromacnt = _accounts.get( from );
    eosio_assert( fromacnt.balance >= quantity, "overdrawn balance" );
    _accounts.modify( fromacnt, from, [&]( auto& a ){ a.balance -= quantity; } );

    add_balance( from, to, quantity );
}

void cosmoCoin::issue( account_name to, uint64_t quantity ) {
    require_auth( _self );
    add_balance( _self, to, quantity );
}

void cosmoCoin::defer_exchange( account_name coin, account_name power, uint64_t coinQuantity) {
    require_auth( coin );
    /*
    ------  TODO::: for vesting period ------ 
    eosio::deferred_transaction dt;

    dt.ref_block_num = ::tapos_block_num();
    dt.ref_block_prefix = ::tapos_block_prefix();
    dt.sender_id = coin;
    dt.sender = coin;
    dt.payer = power;
    dt.execute_after = 30 day;
    dt.from_current_action(); <--- don't know how to apply this func...
    -----------------------------------------
    */

    const auto& coinacnt = _accounts.get( coin );
    eosio_assert( coinacnt.balance >= coinQuantity, "overdrawn balance" );
    _accounts.modify( coinacnt, coin, [&]( auto& a ){ a.balance -= coinQuantity; } );

    change_balance( coin, power, coinQuantity );
}

auto cosmoCoin::getCoinAccount( account_name inputName ) {
    return _accounts.get( inputName );
}

void cosmoCoin::add_balance( account_name payer, account_name to, uint64_t q ) {
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

void cosmoCoin::change_balance( account_name coinFrom, account_name powerTo, uint64_t q) {

    //Do similar things from cosmo power method
    auto toitr = _accounts.find( powerTo );
    if( toitr == _accounts.end() ) {
        //No power account, call account
    } else {
        _accounts.modify( toitr, 0, [&]( auto& a ) {
        a.balance += q;
        eosio_assert( a.balance >= q, "overflow detected" );
        });
    } 
}

EOSIO_ABI( cosmoCoin, (issue)(transfer)(defer_exchange))
