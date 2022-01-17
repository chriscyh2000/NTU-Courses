import Header from './Header';
import Body from './Body';
import styled from 'styled-components';
import Paper from '@material-ui/core/Paper';

const Wrapper = styled.div`
  margin: auto;
  width: 100%;
  height: 100vh;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
`;

const StyledPaper = styled(Paper)`
  padding: 2em;
`;

function App() {
  return (
    <Wrapper>
      <StyledPaper elevation={3}>
        <Header />
        <Body />
      </StyledPaper>
    </Wrapper>
  );
}

export default App;
