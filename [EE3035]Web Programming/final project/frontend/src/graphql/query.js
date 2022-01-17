import { gql } from '@apollo/client';
export const USER_QUERY = gql`
    query user(
        $name: String!
    ){
        user(name: $name){
            password
            favs{
                id
                name
                posts{
                    id
                    title
                    body
                    comments{
                        img
                        sender
                        text
                    }
                }
            }
        }
    }
        
`
// ID or String ???
export const COMMENTS_QUERY = gql`
    query comments(
        $post_id: ID!
    ){
        comments(post: $post_id){
            id
            img
            text
            sender
            post
        }
    }
`

export const POSTS_QUERY = gql`
    query posts(
        $store_id: ID!
    ){
        posts(query: $store_id){
            pimg
            id
            author
            title
            body
            comments{
                sender
                text
                img
            }
            store{
                id
                name
            }
        }
    }
`

export const POST_QUERY = gql`
    query post(
        $post_id: ID!
    ){
        post(postId: $post_id){
            img
            pimg
            title
            body
            author
            store{
                id
                name
            }
            score
            comments{
                img
                sender
                text
            }
        }
    }
`

export const STORES_QUERY = gql`
    query{ 
        stores{
            id
            name
            score
            location
            postCount
            posts{
                title
                body
            }
        }
    }
`

// export const STORE_QUERY = gql`
//     query store(
//         $name: String!
//     ){
//         store(name: $name){
//             id
//             name
//             location
//         }
//     }
// `