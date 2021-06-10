import Container from 'react-bootstrap/Container';

import { BrowserRouter, Route, Switch } from 'react-router-dom';

import ProjectList from './ProjectList';
import Project, { RouterComponentProjectProps } from './Project';
import Structure, { RouterComponentStructureProps } from './Structure';

const App = () => {
   return (
      <Container className="p-3">
         <BrowserRouter>
            <Switch>
               <Route exact path="/404">not found</Route>
               <Route exact path="/"><ProjectList /></Route>
               <Route
                  exact
                  path="/project/:projectId"
                  render={({ match }: RouterComponentProjectProps) => (<Project projectId={match.params.projectId} />)}
               />
               <Route
                  exact
                  path="/structure/:projectId"
                  render={({ match }: RouterComponentStructureProps) => (<Structure projectId={match.params.projectId} />)}
               />
            </Switch>
         </BrowserRouter>
      </Container>
   );
};

export default App;
