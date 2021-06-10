import { useEffect, useState } from "react";
import { Spinner, Table, Alert, Row, Container } from "react-bootstrap";
import { DbLoadCase } from "./DatabaseInterface";
import CopyButton from "./ui/CopyButton";
import EditButton from "./ui/EditButton";
import RemoveButton from "./ui/RemoveButton";
import axios from 'axios';
import NewButton from "./ui/NewButton";

const LoadCaseListItem = (props: DbLoadCase) => {

   const onEdit = () => {
      //! \todo
   }
   const onCopy = () => {
      //! \todo
   }
   const onDelete = () => {
      //! \todo
   }
   return (
      <tr>
         <td><EditButton onClick={onEdit} disabled={true}></EditButton></td>
         <td>{props.name}</td>
         <td>{props.description}</td>
         <td>{props.actionType}</td>
         <td>{props.type}</td>
         <td><CopyButton onClick={onCopy} disabled={true} /></td>
         <td><RemoveButton onClick={onDelete} disabled={true} /></td>
      </tr>
   )
}

export type LoadCaseListProps = {
   projectId: string;
}

const LoadCaseList = (props: LoadCaseListProps) => {

   const [data, setData] = useState<DbLoadCase[]>([])
   const [loading, setLoading] = useState(true)

   async function getData() {
      await axios.get(process.env.REACT_APP_BACKEND + '/loadcases/' + props.projectId)
         .then(response => setData(response.data))
         .finally(() => setLoading(false))
   }

   useEffect(() => { getData() }, []);

   if (loading) {
      return (<><Spinner animation="border" role="status"></Spinner></>);
   }

   return (
      <Container>
         <Row>
            <h4>Loadcases</h4>
            <NewButton onClick={() => { }} disabled={true} />
         </Row>
         {data.length === 0 && <Alert variant='danger'>No loadcases defined.</Alert>}
         <Table borderless hover size="sm">
            <thead>
               <tr>
                  <th></th>
                  <th>Name</th>
                  <th>Description</th>
                  <th>Action</th>
                  <th>Type</th>
               </tr>
            </thead>
            <tbody>
               {data.map(e => (
                  <LoadCaseListItem
                     key={e.id}
                     id={e.id}
                     name={e.name}
                     uuid={e.uuid}
                     description={e.description}
                     type={e.type}
                     actionType={e.actionType}
                  />))}
            </tbody>
         </Table>
      </Container>
   )
}

export default LoadCaseList;