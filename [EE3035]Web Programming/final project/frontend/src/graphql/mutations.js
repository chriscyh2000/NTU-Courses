import { gql } from '@apollo/client';
const CREATECOMMENT_MUTATION = gql`
    mutation createComment(
        $sender: ID!
        $text: String!
        $post: ID!
    ){
        createComment(data: {
            sender: $sender
            text: $text
            post: $post
        }){
            id
            img
            text
            sender
            post
        }
    }
`

const CREATEPOST_MUTATION = gql`
    mutation createPost(
        $title: String!
        $body: String!
        $score: Int!
        $author: String!
        $store: ID!
        $img: String
        $pimg: String
    ){
        createPost(data: {
            title: $title
            body: $body
            score: $score
            author: $author
            store: $store
            img: $img
            pimg: $pimg
        }){
            id
            author
            title
            body
            store{
                id
                name
            }
        }
    }
`

const CREATESTORE_MUTATION = gql`
    mutation createStore(
        $name: String!
        $location: String!
    ){
        createStore(data: {
            name: $name
            location: $location
        }){
            id
            name
            location
            posts{
                id
                author
                body
                comments{
                    id
                    sender
                    post
                    text
                }
                store{
                    id
                    name
                }
            }
        }
    }
`

const LOGIN_MUTATION = gql`
    mutation login(
        $name: String!
        $password: String!
    ){
        login(name: $name, password: $password){
            message
            user {
                name
                id
                password
                favs{
                    id
                    name
                    score
                    postCount
                    location
                    posts{
                        id
                        title
                        body
                        comments{
                            sender
                            text
                        }
                        store{
                            id
                            name
                        }
                    }
                }
                posts{
                    id
                    store{
                        id
                        name
                    }
                    author
                    body
                    comments{
                        id
                        sender
                        text
                    }
                }
                biography
                image
            }
        }
    }
`

const SIGNUP_MUTATION = gql`
    mutation signup(
        $name: String!
        $password: String!
    ){
        signup(name: $name, password: $password){
            message
            password
        }
    }
`

const FINDSTORE_MUTATION = gql`
    mutation findStore(
        $name: String!
    ){
        findStore(name: $name){
            id
        }
    }
`

const FINDSTOREBYID_MUTATION = gql`
    mutation findStoreById(
        $id: ID!
    ){
        findStoreById(id: $id){
            name
        }
    }
`

const EDITPROFILE_MUTATION = gql`
    mutation editProfile(
        $userId: ID!
        $bio: String!
    ){
        editProfile(userId: $userId, biography: $bio){
            name
        }
    }
`
const ADDFAV_MUTATION = gql`
    mutation addFav(
        $store_id: ID!
        $user_id: ID!
    ){
        addFav(storeId: $store_id, userId: $user_id){
            name
        }
    }
`
const REMOVEFAV_MUTATION = gql`
    mutation removeFav(
        $store_id: ID!
        $user_id: ID!
    ){
        removeFav(storeId: $store_id, userId: $user_id){
            name
        }
    }
`

const UPLOADUSERIMAGE_MUTATION = gql`
    mutation uploadUserImage( 
        $name: String!
        $image: String!
    ){
        uploadUserImage(name: $name, image: $image)
    }
`

export { SIGNUP_MUTATION, LOGIN_MUTATION, CREATEPOST_MUTATION, FINDSTORE_MUTATION, 
        CREATESTORE_MUTATION, EDITPROFILE_MUTATION, ADDFAV_MUTATION, 
        REMOVEFAV_MUTATION, CREATECOMMENT_MUTATION, FINDSTOREBYID_MUTATION, UPLOADUSERIMAGE_MUTATION };