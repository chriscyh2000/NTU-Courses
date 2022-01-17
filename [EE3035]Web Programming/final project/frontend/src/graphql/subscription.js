import { gql } from '@apollo/client';
export const POST_SUBSCRIPTION = gql`
    subscription post(
        $store_id: ID!
    ){
        post(id: $store_id){
            data{
                pimg
                img
                title
                body
                id
                author
            }
        }
    }
`
export const COMMENT_SUBSCRIPTION = gql`
    subscription comment(
        $post_id: ID!
    ){
        comment(id: $post_id){
            data{
                img
                text
                sender
            }
        }
    }
`

export const STORES_SUBSCRIPTION = gql`
    subscription {
        store{
            mutation
            data{
                id
                name
                location
                score
                postCount
                posts{
                    id
                    title
                    body
                    comments{
                        img
                        id
                        sender
                        text
                    }
                }
            }
        }
    }
`