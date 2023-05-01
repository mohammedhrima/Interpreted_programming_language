prime:
    func:
        skip identifier
        skip (
        skip idenitifer then )
        skip :
        bloc
    
    identifier:
        if next is (: (means it's a function call)
            expr
            keep skiping comma
        
        if next is [ : (means it's iteration by index)
            expect 'number' or 'add_sub'
        
        if next is '.' : (means it's iteration by key or attribute)
            while '.' continue
            get prime (should be identifier)
        
        if next is ++ --:
            increment / decrement by one
    
    indexof:
        skip (:
            get prime (one prime)
    
    output:
        skip (:
            get expr
            skip commas
    
    
    array []: keep skiping comma
    (): don't skip comma, return expr
    obj {}: prime + : + value + comma

    characters || number || boolean || break || continue || type
        return it

get len, isalpha, isnumber ... as identifiers and check them in evaluation

- in evaluating in function declaration, skip check step for right is identifier, because right can get value in future
